package shapesexample;

public class NhanVien {
    private String tenNhanVien;
    private double luongCoBan;
    private double heSoLuong;
    public final double LUONG_MAX= 3e7;

    public NhanVien() {
    }

    public String getTenNhanVien() {
        return tenNhanVien;
    }

    public void setTenNhanVien(String tenNhanVien) {
        this.tenNhanVien = tenNhanVien;
    }

    public double getLuongCoBan() {
        return luongCoBan;
    }

    public void setLuongCoBan(double luongCoBan) {
        this.luongCoBan = luongCoBan;
    }

    public double getHeSoLuong() {
        return heSoLuong;
    }

    public void setHeSoLuong(double heSoLuong) {
        this.heSoLuong = heSoLuong;
    }

    public double tinhLuong(){
        return this.luongCoBan*this.heSoLuong;
    }

    public boolean tangLuong(double k){
        if(this.luongCoBan*k > LUONG_MAX){
            System.out.println("Luong > Luong max\n");
            return false;
        }else {
            this.heSoLuong = k;
            return true;
        }
    }

    public void inTTin(){
        System.out.println("Ten: " + this.tenNhanVien);
        System.out.println("Luong co ban: " + this.luongCoBan);
        System.out.println("He so luong: " + this.heSoLuong);
        System.out.println("Luong nhan: " + tinhLuong());
    }

}
