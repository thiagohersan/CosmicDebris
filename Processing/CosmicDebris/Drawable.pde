
public class Drawable implements Objectable{
  private static final int TYPE_CIRCLE = 0;
  private static final int TYPE_SQUARE = 1;
  private static final int TYPE_HOR_RECT = 2;
  private static final int TYPE_VER_RECT = 3;
  private static final int TYPE_CROSS = 4;
  private static final int TYPE_KAHANE = 6;
  //  public static final int TYPE_UNO = 5;

  private final int[] TYPES = {
    Drawable.TYPE_CIRCLE, Drawable.TYPE_SQUARE, Drawable.TYPE_HOR_RECT, Drawable.TYPE_VER_RECT, Drawable.TYPE_CROSS, Drawable.TYPE_KAHANE
  };


  float mySize;
  //int mx, my;
  int myType;

  public Drawable() {
    mySize = 0.0;
    myType = TYPE_CIRCLE;
  }

  public void setType(int t) {
    myType = TYPES[t%TYPES.length];
  }  

  public void setSize(float f) {
    mySize = f;
  }  

  public void draw(float mx, float my) {
    draw(mx, my, 0, color(255, 255, 255));
  }
  public void draw(float mx, float my, color c) {
    draw(mx, my, 0, c);
  }


  public void draw(float mx, float my, float var, color c) {
    if (myType == TYPE_CIRCLE) {
      noStroke();
      int rad = (int)map(mySize, 0, 1, 20, 400);
      int a = (mySize<0.25)?255:(int)map(mySize, 0, 1, 255, 0);
      fill(c, a);
      ellipse(mx+random(-var, var), my+random(-var, var), 2*rad, 2*rad);
    }
    ////
    else if (myType == TYPE_SQUARE) {
      noStroke();
      int rad = (int)map(mySize, 0, 1, 50, 400);
      int a = (mySize<0.8)?255:(int)map(mySize, 0, 1, 200, 80);
      fill(c, a);
      rect(mx+random(-var, var), my+random(-var, var), rad, rad);
    }
    ////
    else if (myType == TYPE_VER_RECT) {
      noStroke();
      int rad = (int)map(mySize, 0, 1, 50, 400);
      int a = (mySize<0.3)?255:(int)map(mySize, 0, 1, 255, 0);
      fill(c, a);
      rect(mx+random(-var, var), my+random(-var, var), rad, 2*rad);
    }
    ////
    else if (myType == TYPE_HOR_RECT) {
      noStroke();
      int rad = (int)map(mySize, 0, 1, 50, 400);
      int a = (mySize<0.3)?255:(int)map(mySize, 0, 1, 255, 0);
      fill(c, a);
      rect(mx+random(-var, var), my+random(-var, var), 2*rad, rad);
    }
    else if (myType == TYPE_CROSS) {
      noStroke();
      int rad = (int)map(mySize, 0, 1, 50, 400);
      int a = (mySize<0.2)?255:(int)map(mySize, 0, 1, 255, 0);
      fill(c, a);
      float r0 = random(-var, var);
      float r1 = random(-var, var);
      rect(mx+r0, my+r1, rad, 2*rad);
      rect(mx+r0, my+r1, 2*rad, rad);
    }
    else if (myType == TYPE_KAHANE) {
      noStroke();
      int rad = (int)map(mySize, 0, 1, 50, 400);
      int a = (mySize<0.2)?255:(int)map(mySize, 0, 1, 255, 0);
      fill(c, a);
      float r0 = random(-var, var);
      float r1 = random(-var, var);
      float r2 = random(-var, var);
      float r3 = random(-var, var);

      triangle(mx+r0, my+r1-rad, mx+r0-rad, my+r1+rad, mx+r0+rad, my+r1+rad);
      triangle(mx+r2, my+r3+rad, mx+r2-rad, my+r3-rad, mx+r2+rad, my+r3-rad);
    }
  }
}

