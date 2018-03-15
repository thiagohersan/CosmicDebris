// EXPOSE: color
//         radius (0,100)
//         target var
//         alpha (independent of size?)
public class Organism {
  private PVector loc, vel;
  private float r;
  private int myColor;

  private float targetVar;

  public Organism() {
    r = 30;
    loc = new PVector(random(width), random(height), 0);
    vel = new PVector(random(-4, 4), random(-4, 4), 0);

    myColor = 0xffFFFFFF;
    targetVar = random(-2,2);
  }

  public PVector getLoc() {
    return loc;
  }

  public void setColor(int c) {
    myColor = c;
  }

  //
  public boolean isDead() {
    // if out of bounds... or speed = 0
    if ((loc.x-r>width) || (loc.x<-r) || (loc.y-r>height) || (loc.y<-r) || (vel.mag() < 0.5)) {
      return true;
    }
    return false;
  }

  // 
  public void setTarget(PVector tv) {
    // unit vector towards target??
    vel = PVector.sub(loc, tv);
    if (vel.mag() > 1) {
      vel.normalize();
    }
    vel.mult(targetVar);
  }

  //
  public void update() {
    loc.add(vel);
  }

  //
  public void draw() {
    pushMatrix();
    translate(loc.x, loc.y);
    noStroke();
    fill(myColor);
    ellipse(0, 0, 2*r, 2*r);
    popMatrix();
  }
}

