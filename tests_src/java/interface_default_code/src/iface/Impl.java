package iface;

/**
 * Impl.greet() compiles to invokespecial on iface/Greeter.greet, whose declaring class is an
 * interface. Resolving that dispatch asks whether the interface is a subclass of Impl.
 */
public class Impl implements Greeter {
    @Override
    public String greet() {
        return Greeter.super.greet();
    }

    public static void main(String[] args) {
        System.out.println(new Impl().greet());
    }
}
