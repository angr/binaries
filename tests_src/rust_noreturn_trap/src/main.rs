use std::collections::HashMap;

#[derive(Clone, Copy)]
struct Point {
    x: u64,
    y: u64,
}

fn fold(pts: &[Point], n: u64) -> u64 {
    let mut acc = n;
    let mut tally: HashMap<u64, u64> = HashMap::new();
    for p in pts {
        let v = p.x.wrapping_mul(6725).wrapping_add(p.y).wrapping_add(acc);
        acc ^= v.wrapping_add(v >> 7);
        *tally.entry(v % 16).or_insert(0) = tally.get(&(v % 16)).copied().unwrap_or(0).wrapping_add(acc);
    }
    for kk in 0u64..16 {
        acc = acc.wrapping_add(tally.get(&kk).copied().unwrap_or(0).wrapping_mul(kk + 1));
    }
    acc
}

fn main() {
    let n: u64 = std::env::args().nth(1)
        .and_then(|s| s.parse().ok())
        .unwrap_or(21);
    let pts = [Point{x:945583970,y:945617109}, Point{x:884821196,y:884851579}, Point{x:307150830,y:307114073}, Point{x:479482170,y:479518349}, Point{x:365883735,y:365854432}, Point{x:211983533,y:211948314}, Point{x:506168172,y:506137819}, Point{x:310302115,y:310270484}, Point{x:253831777,y:253863382}];
    println!("{}", fold(&pts, n));
}
