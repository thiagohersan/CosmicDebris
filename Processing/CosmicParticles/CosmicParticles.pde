import processing.opengl.*;

World w;

void setup() {
  size(800, 800,OPENGL);
  smooth();
  w = new World();
}

void draw() {
  // DEBUG
  if (frameCount %100 == 0) {
    println(frameRate);
  }

  background(0);
  w.update();
  w.draw();
  fill(0, 0, 0, 10);
  //rect(0, 0, width, height);
}

