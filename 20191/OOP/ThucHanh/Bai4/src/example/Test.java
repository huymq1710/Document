package example;

import java.util.Scanner;

public class Test {

    public static void change1(Student std){
        std.setYear(2000);
    }

    public static void change2(Student std){
        std=new Student("Hung",2010);
    }

    public static void main(String[] args){
/*
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        Student arr[] = new Student[n];
        for(int i=0;i<n;i++){
            String name = in.next();
            int year = in.nextInt();
            arr[i] = new Student(name,year);
        }

        int sumAge=0;
        for(int i=0;i< arr.length;i++){
            int stt =i+1;
            System.out.println(stt +": "+ arr[i].getName() + "-"+arr[i].getYear() );
            sumAge+=arr[i].getYear();
        }
        System.out.println("Sum Age: "+sumAge);

 */
        Student std1 = new Student("Nam",1990);
        System.out.println(std1.getYear());
        change1(std1);
        System.out.println(std1.getYear());

        Student std2 = new Student("Nam",1990);
        System.out.println(std2.getYear());
        change2(std2);
        System.out.println(std2.getYear());

    }
}
