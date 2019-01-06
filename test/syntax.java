class Factorial {
    public static void main(String[] a){
        int a;
        a = 1*;
        int b;
        b = *2;
        System.out.println(new Fac().ComputeFac(10));
    }
}

class Fac {
    public int ComputeFac(int num) {
        int num_aux;
        if (num < 1) {
            num_aux = 0;
        }
        else {
            num_aux = num * (this.ComputeFac(num - 1)) ;
        }
        return num_aux ;
    }
}
