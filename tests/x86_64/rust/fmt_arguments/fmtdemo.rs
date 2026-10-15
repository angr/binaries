use std::fmt::Write as _;
use std::io::Write as _;

#[derive(Debug)]
struct Point { x: i32, y: i32 }

#[inline(never)]
fn print_one(n: u64) { println!("value = {}", n); }

#[inline(never)]
fn print_two(a: &str, p: &Point) { println!("name {} at {:?}!", a, p); }

#[inline(never)]
fn print_const() { println!("just a literal"); }

#[inline(never)]
fn eprint_debug(e: &std::io::Error) { eprintln!("Error: {:?}", e); }

#[inline(never)]
fn make_string(a: u32, b: &str) -> String { format!("{}-{}", a, b) }

#[inline(never)]
fn hex_width(v: u32) -> String { format!("0x{:08x} {:>5}", v, v) }

#[inline(never)]
fn reorder(a: u32, b: u32) { println!("{1} {0} {1}", a, b); }

#[inline(never)]
fn write_out(out: &mut dyn std::io::Write, v: i64) -> std::io::Result<()> { writeln!(out, "v: {}", v) }

#[inline(never)]
fn write_str(s: &mut String, v: i64) { write!(s, "[{}]", v).unwrap(); }

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let n = args.len() as u64;
    print_one(n);
    print_two(&args[0], &Point { x: n as i32, y: 2 });
    print_const();
    eprint_debug(&std::io::Error::other("x"));
    println!("{}", make_string(n as u32, &args[0]));
    println!("{}", hex_width(n as u32));
    reorder(n as u32, 7);
    write_out(&mut std::io::stdout(), n as i64).unwrap();
    let mut s = String::new();
    write_str(&mut s, n as i64);
    std::io::stdout().write_all(s.as_bytes()).unwrap();
}
