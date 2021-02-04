package shapesexample;

public class TheCircle {
    private int r;
    private String owner;

    public TheCircle(int r, String owner) {
        this.r = r;
        this.owner = owner;
    }

    public void setR(int r) {
        this.r = r;
    }

    public void setOwner(String owner) {
        this.owner = owner;
    }

    public int getR() {
        return r;
    }

    public String getOwner() {
        return owner;
    }

}
