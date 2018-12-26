class Testbed {
    public static void main(String[] x){
        System.out.println(new Foo().Excite(5));
    }
}

class Foo extends Bar {
    public int Excite(int x) {
        int y;
        if (x < 1)
            y = 1;
          else 
            y = x * (this.Excite(x-1));
        Bar z;
        return y;
    }
}

class Bar extends Foo {
    public int ExciteY(int x) {
        return 0;
    }
}
