import java.util.Scanner;
import shapesexample.Vector;
import shapesexample.NhanVien;

public class Main {
    public static void main(String[] args){
        Scanner in =new Scanner(System.in);

        Vector a1 = new Vector(2,3);
        Vector a2 = new Vector(4,5);
        Vector c = a1.congVector(a1,a2);
        System.out.println("Vector c"+ c.getX() + " "+ c.getY());

        NhanVien nv1 = new NhanVien();
        nv1.setTenNhanVien("Mac Quang Huy");
        nv1.setLuongCoBan(1e6);
        nv1.setHeSoLuong(1);

        System.out.println("Luong nhan: "+nv1.tinhLuong());
        nv1.inTTin();

        nv1.tangLuong(3);
        nv1.inTTin();

        nv1.tangLuong(40);
        nv1.inTTin();




    }
}
