class Testbed {
    public static void main(String[] x){
        System.out.println(new Foo().Excite(new int[][5]));
    }
}

class Foo {
    int [][] im2d;
    public int Excite(int[][] x) {
        im2d = new int[] [3];
        im2d[1] = new int[5];
        int y;
        Bar z;
        if (x < 1)
            y = 1;
          else
            y = x * (this.Excite(x-1));
        return y;
    }
}

class Bar extends Foo {
    public int ExciteY(int x) {
        Foo[] arr;
        arr = new Foo[5];
        arr[1] = new Foo();
        return 0;
    }
}


