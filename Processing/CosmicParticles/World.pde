// EXPOSE: NUM_ORGS
//         NUM_GROUPS
public class World {
  private final int NUM_ORGS = 1000;
  private final int NUM_GROUPS = 4;

  private ArrayList<Organism> theOrgs;

  private int avgGroupSize = NUM_ORGS/NUM_GROUPS;
  private ArrayList<PVector> theAvgs;

  public World() {
    theOrgs = new ArrayList<Organism>();

    for (int i=0; i<NUM_ORGS; i++) {
      theOrgs.add(new Organism());
    }

    theAvgs = new ArrayList<PVector>();
    for (int i=0; i<NUM_GROUPS; i++) {
      theAvgs.add(new PVector(random(width), random(height)));
    }
  }

  // update 
  // just pick n random locations for the averages....
  public void update() {
    // compute sum and clean up dead organisms
    for (int i=theOrgs.size()-1; i>-1; i--) {
      Organism o = theOrgs.get(i);
      if (o.isDead() == true) {
        theOrgs.remove(i);
        o = new Organism();
        theOrgs.add(o);
      }
      /////
      PVector groupTarget = theAvgs.get(i/avgGroupSize);
      //PVector avg = PVector.div(groupSum, avgGroupSize);
      o.setTarget(groupTarget);
      o.update();
    }
  }

  ////
  public void draw() {
    for (Organism o : theOrgs) {
      o.draw();
    }
  }

  //
}

