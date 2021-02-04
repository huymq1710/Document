package shapesexample;

public class Vector {
    float x;
    float y;

    public Vector(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public float getX() {
        return x;
    }

    public void setX(float x) {
        this.x = x;
    }

    public float getY() {
        return y;
    }

    public void setY(float y) {
        this.y = y;
    }

    public Vector congVector(Vector a, Vector b){
        Vector c =new Vector(a.x+b.x,a.y+b.y);
        return c;
    }
}
