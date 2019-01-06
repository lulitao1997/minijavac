class Testbed {
    public static void main(String[] x){
        System.out.println(new Foo().Excite(5));
    }
}

class Foo {
    public int Excite(int x) {
        int y;
        Bar z;
        if (x < 1)
            y = 1;
          else 
            y = x * (this.Excite());
        return y;
    }
    public int Excite(int x, int y) {
    }

}

class Bar extends Foo {

    public int Excite(int y) {

    }


    public int Excite(int x) {
        int a;
        {
            int a;

            int a;
        }
        return 0;
    }
}
