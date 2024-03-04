let sliders = [];
let knobs = [];
let dragging = [];
let serial;

function setup() {
  createCanvas(windowWidth, windowHeight);
  for (let i = 0; i < 16; i++) {
    let angle = 2*PI/16 * i;
    sliders.push(new Slider(500 + 200*cos(angle),
                            500 + 200*sin(angle),
                            0,
                            angle ));
    knobs.push(new Knob(500 + 350*cos(angle),
                        500 + 350*sin(angle)));
  }
  serial = new SerialConnection();
}

async function sendParams() {
  if (serial.connected) {
    let phase_array = new Uint8Array(sliders.length+1);
    phase_array[0] = 254 & 0xFF;
    for (let i = 0; i < sliders.length; i++) {
      phase_array[i+1] = round(sliders[i].val/255*31) & 0xFF;
    }
    let amp_array = new Uint8Array(knobs.length+1);
    amp_array[0] = 254 & 0xFF;
    for (let i = 0; i < knobs.length; i++) {
      amp_array[i+1] = round(knobs[i].val/255*15) & 0xFF;
    }
    await serial.write(phase_array);
    await serial.write(amp_array);
  }
}

function draw() {
  background(200);
  noStroke();
  fill(255)
  rect(100,100, 800, 800, 50);
  serial.draw();
  for (let i = 0; i <sliders.length; i ++) {
    if (dragging.includes(sliders[i])){
      sliders[i].mouse2pos();
    }
    if (dragging.includes(knobs[i])){
      knobs[i].mouse2pos();
    }
    sliders[i].draw()
    knobs[i].draw()
  }
}

function mousePressed() {
  for (let i = 0; i <sliders.length; i ++) {
    if (sliders[i].mouseOver()){
      dragging.push(sliders[i]);
    }
    if (knobs[i].mouseOver()){
      knobs[i].start_val = knobs[i].val;
      dragging.push(knobs[i]);
    }
  }
  if(serial.mouseOver()) {
    serial.connect()
  }
}

  function mouseReleased() {
    if (dragging.length > 0) {
      dragging = [];
      sendParams();
    }
  }

class Slider {

  constructor(x, y, val = 0, rotation = 0) {
    this.x = x;
    this.y = y;
    this.val = val;
    this.length = 200;
    this.slider_width = 10;
    this.slider_height = 30;
    this.rotation = rotation;

  }

  draw() {
    resetMatrix();
    translate(this.x, this.y);
    rotate(this.rotation);
    fill(150);
    noStroke();
    rect(-this.length/2,-5,this.length,10,5);

    fill(0);
    let slider_center = createVector( - this.length/2 + (this.val/255)*this.length,
                                     0);
    fill(0);
    rect(slider_center.x - this.slider_width/2,
          - this.slider_height/2,
         this.slider_width, this.slider_height,
         2.5);
    stroke(255);
    strokeWeight(2);
    line(slider_center.x,
         -this.slider_height/2,
         slider_center.x,
         this.slider_height/2);
  }

  ConvertCoords(x,y){
    let tempvect = createVector(mouseX - this.x, mouseY - this.y);
    let coss = cos(-this.rotation);
    let sinn = sin(-this.rotation);
    return createVector(tempvect.x*coss-tempvect.y*sinn, tempvect.y*coss+tempvect.x*sinn);
  }

  mouseOver(){
    let new_coord = this.ConvertCoords(mouseX,mouseY);
    return new_coord.x >= - this.length/2  &&
           new_coord.x <= this.length/2 &&
           new_coord.y >= - this.slider_height/2 &&
           new_coord.y <= this.slider_height/2;
  }

  mouse2pos() {
    let new_coord = this.ConvertCoords(mouseX,mouseY);
    this.val = min(this.length, max(0, new_coord.x - (- this.length/2)))/this.length *255;
  }
}


class Knob {
  constructor(x, y, val=0) {
    this.x = x;
    this.y = y;
    this.radius = 30;
    this.val = val;
    this.start_val = val;
  }

  draw() {
    resetMatrix();
    translate(this.x, this.y);
    rotate(-3*PI/4 + (this.val/255)*6*PI/4);
    noStroke();
    fill(0);
    circle(0, 0, this.radius);
    stroke(255);
    
    line(0, 0-(this.radius + 5), 0, 0);
  }

  mouseOver(){
    return (mouseX-this.x)**2 + (mouseY-this.y)**2 < this.radius**2
  }

  mouse2pos() {
    this.val = max(0, min(255, this.start_val + (this.y-mouseY)));
  }
}

class SerialConnection {
  constructor(){
    this.connected = false
    this.port = false
  }

  draw(){
    resetMatrix();
    noStroke();
    textSize(20);
    if (this.connected) {
      fill(255,100,100);
      rect(100, 30, 120, 50, 10)
      fill(0);
      text('Disconnect', 110, 60);
    }
    else {
      fill(100,255,100);
      rect(100, 30, 100, 50, 10)
      fill(0);
      text('Connect', 110, 60);
    }
  }

  mouseOver(){
    return mouseX >=  100  &&
           mouseX <= 200 &&
           mouseY >= 30 &&
           mouseY <= 80;
  }

  async check_connect(){
    if (this.connected) {
      let reader = this.port.readable.getReader();
      const { value, done } = await reader.read();
      if (done) {
        // Allow the serial port to be closed later.
        this.connected = false;
        reader.releaseLock();
      }
      reader.releaseLock();
    }
  }


  async connect(){
    if (this.connected) {
      await this.port.close()
      this.connected = false;
    } else {
      this.port = await navigator.serial.requestPort();
      await this.port.open({ baudRate: 115200 });
      this.connected = true;
    }
  }

  async write(data) {
    console.log(data);
    const writer = this.port.writable.getWriter();
    try {
        let result = await writer.write(data);
    } catch(error) {
        console.error(error);
    }
    writer.releaseLock();
  }
}