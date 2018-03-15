public class Morphable implements Objectable {
  private final int TYPE_SQUARE = 0;
  private final int TYPE_CIRCLE = 1;
  private static final int TYPE_HOR_RECT = 2;
  private static final int TYPE_VER_RECT = 3;
  private static final int TYPE_CROSS = 4;


  private int[] TYPES = {
    TYPE_SQUARE, TYPE_CIRCLE, TYPE_HOR_RECT, TYPE_VER_RECT, TYPE_CROSS
  };

  private final int STATE_STEADY = 0;
  private final int STATE_MORPHING = 1;

  private final int MORPH_PERIOD = 10;

  private float currSize, targetSize;

  private int currType, targetType;
  private ArrayList<PVector> currPoints, targetPoints;
  // arrayList of bezier points... 2 per vertex...
  private ArrayList<PVector> leftBez, rightBez;

  private int currState;
  private long lastMorph;
  private long noiseVar;

  public Morphable() {
    currState = STATE_STEADY;
    currType = -1;
    targetType = currType;

    currSize = 50.0;
    targetSize = currSize;

    lastMorph = millis();
    noiseVar = 0;

    currPoints = new ArrayList<PVector>();
    targetPoints = new ArrayList<PVector>();
    leftBez = new ArrayList<PVector>();
    rightBez = new ArrayList<PVector>();

    for (int i=0; i<12; i++) {
      currPoints.add(new PVector(0, 0));
      targetPoints.add(new PVector(0, 0));
      leftBez.add(new PVector(0, 0));
      rightBez.add(new PVector(0, 0));
    }

    setType(0);
  }

  // t is index into type array
  public void setType(int t) {
    if (TYPES[t%TYPES.length] != currType) {
      targetType = TYPES[t%TYPES.length];

      for (int i=0; i<12; i++) {
        leftBez.set(i, new PVector(0, 0));
        rightBez.set(i, new PVector(0, 0));
      }

      // set up square
      if (targetType == TYPE_SQUARE) {
        // make a square...
        //   three squares on top of each other ...

        // TODO: use radians!!
        targetPoints.set(0, new PVector(-1, -1));
        targetPoints.set(1, new PVector(-1/3, -1));
        targetPoints.set(2, new PVector(1/3, -1));

        targetPoints.set(3, new PVector(1, -1));
        targetPoints.set(4, new PVector(1, -1/3));
        targetPoints.set(5, new PVector(1, 1/3));

        targetPoints.set(6, new PVector(1, 1));
        targetPoints.set(7, new PVector(1/3, 1));
        targetPoints.set(8, new PVector(-1/3, 1));

        targetPoints.set(9, new PVector(-1, 1));
        targetPoints.set(10, new PVector(-1, 1/3));
        targetPoints.set(11, new PVector(-1, -1/3));
      }

      else if (targetType == TYPE_CIRCLE) {
        // use radians...??
        for (int i=0; i<12; i+=4) {
          targetPoints.set(i+3, new PVector(-1, -1));
          leftBez.set(i+3, new PVector(-0.5, 0.5));
          rightBez.set(i+3, new PVector(0.5, -0.5));
          //
          targetPoints.set(i+0, new PVector(1, -1));
          leftBez.set(i+0, new PVector(-0.5, -0.5));
          rightBez.set(i+0, new PVector(0.5, 0.5));
          //
          targetPoints.set(i+1, new PVector(1, 1));
          leftBez.set(i+1, new PVector(0.5, -0.5));
          rightBez.set(i+1, new PVector(-0.5, 0.5));
          //
          targetPoints.set(i+2, new PVector(-1, 1));
          leftBez.set(i+2, new PVector(0.5, 0.5));
          rightBez.set(i+2, new PVector(-0.5, -0.5));
        }
      }

      else if (targetType == TYPE_HOR_RECT) {
        // use radians!!
        for (int i=0; i<12; i+=4) {
          targetPoints.set(i+2, new PVector(-1, -0.5));
          //
          targetPoints.set(i+3, new PVector(1, -0.5));
          //
          targetPoints.set(i+0, new PVector(1, 0.5));
          //
          targetPoints.set(i+1, new PVector(-1, 0.5));
        }
      }

      else if (targetType == TYPE_VER_RECT) {
        // use radians!!
        for (int i=0; i<12; i+=4) {
          targetPoints.set(i+3, new PVector(-0.5, -1));
          //
          targetPoints.set(i+2, new PVector(0.5, -1));
          //
          targetPoints.set(i+1, new PVector(0.5, 1));
          //
          targetPoints.set(i+0, new PVector(-0.5, 1));
        }
      }

      else if (targetType == TYPE_CROSS) {
        // use radians!!
        targetPoints.set(0, new PVector(-0.5, -1));
        targetPoints.set(1, new PVector(0.5, -1));
        targetPoints.set(2, new PVector(0.5, -0.5));

        targetPoints.set(3, new PVector(1, -0.5));
        targetPoints.set(4, new PVector(1, 0.5));
        targetPoints.set(5, new PVector(0.5, 0.5));
        //
        targetPoints.set(6, new PVector(0.5, 1));
        targetPoints.set(7, new PVector(-0.5, 1));
        targetPoints.set(8, new PVector(-0.5, 0.5));
        //
        targetPoints.set(9, new PVector(-1, 0.5));
        targetPoints.set(10, new PVector(-1, -0.5));
        targetPoints.set(11, new PVector(-0.5, -0.5));
      }

      currState = STATE_MORPHING;
    }
  }

  // s := [0,1], but internal sizes in pixel !!
  public void setSize(float s) {
    if (s != targetSize) {
      targetSize = map(s, 0, 1, 50, 400);
      currState = STATE_MORPHING;
    }
  }

  public float getSize() {
    return currSize;
  }

  public void draw(float mx, float my) {
    draw(mx, my, 0, color(255, 255, 255));
  }

  public void draw(float mx, float my, float var) {
    draw(mx, my, var, color(255, 255, 255));
  }

  public void draw(float mx, float my, float var, color c) {
    if (currState == STATE_MORPHING) {
      morphStep();
    }

    int a = (currSize<100)?255:(int)map(currSize, 100, 400, 200, 10);
    fill(c,a);

    noStroke();

    beginShape();
    for (int i=0; i<currPoints.size(); i++) {
      float vx0 = mx + currSize*currPoints.get(i).x;
      float vy0 = my + currSize*currPoints.get(i).y;
      float vxv0 = noise(vx0, vy0, ++noiseVar*0.001)*currSize/2-currSize/4;
      float vyv0 = noise(vx0, vy0, ++noiseVar*0.001+TWO_PI)*currSize/2-currSize/4;

      float vx1 = mx + currSize*currPoints.get((i+1)%currPoints.size()).x;
      float vy1 = my + currSize*currPoints.get((i+1)%currPoints.size()).y;
      float vxv1 = noise(vx1, vy1, ++noiseVar*0.001)*currSize/2-currSize/4;
      float vyv1 = noise(vx1, vy1, ++noiseVar*0.001+TWO_PI)*currSize/2-currSize/4;

      float cx0 = vx0 + currSize*rightBez.get((i)%rightBez.size()).x;
      float cy0 = vy0 + currSize*rightBez.get((i)%rightBez.size()).y;

      float cx1 = vx1 + currSize*leftBez.get((i+1)%leftBez.size()).x;
      float cy1 = vy1 + currSize*leftBez.get((i+1)%leftBez.size()).y;

      vxv0 = vyv0 = vxv1 = vyv1 = 0;

      vertex(vx0+vxv0, vy0+vyv0);
      bezierVertex(cx0, cy0, cx1, cy1, vx1+vxv1, vy1+vyv1);
    }
    endShape(CLOSE);
  }

  private void morphStep() {
    // figure out if it's time to morph
    if ((millis()-lastMorph) > MORPH_PERIOD) {
      //////////
      // size morph
      if (abs(targetSize - currSize) > 1.0) {
        currSize = 0.9*currSize + 0.1*targetSize;
      }
      /////////
      // type morph
      if (targetType != currType) {
        boolean isSame = true;
        for (int i=0; i<currPoints.size(); i++) {
          // current
          float cx0 = currPoints.get(i).x;
          float cy0 = currPoints.get(i).y;
          // target
          float tx0 = targetPoints.get(i).x;
          float ty0 = targetPoints.get(i).y;
          // update
          cx0 = 0.95*cx0 + 0.05*tx0;
          cy0 = 0.95*cy0 + 0.05*ty0;

          currPoints.get(i).x = cx0;
          currPoints.get(i).y = cy0;

          isSame &= (abs(dist(cx0, cy0, tx0, ty0)) < 0.01);
        }

        // curr got to target
        if (isSame == true) {
          currType = targetType;
        }
      }

      if ((abs(targetSize - currSize) < 1.0) && (targetType == currType)) {
        currState = STATE_STEADY;
      }

      //
      lastMorph = millis();
    }
  }
}  

