package shapesexample;

public class TheSquare {
    private int d;
    private String owner;

    public TheSquare(int d, String owner){
        this.d=d;
        this.owner=owner;
    }

    public void setD(int d){
        this.d=d;
    }

    public void setOwner(String owner){
        this.owner=owner;
    }

    public int getD(){
        return this.d;
    }

    public String getOwner(){
        return this.owner;
    }
}
