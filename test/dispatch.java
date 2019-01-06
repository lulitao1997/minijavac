class Testbed {
    public static void main(String[] x){
        System.out.println(new C().excite(new B()));
        System.out.println(new C().excite(new A()));
    }
}

class A {
    public int excite(A a) {}
}

class B extends A {
    public int excite(B a) {}
}

class C extends B {
    public int excite(A a) {}
}
