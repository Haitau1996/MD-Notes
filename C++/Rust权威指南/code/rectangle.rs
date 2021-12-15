// 最基础的实现
fn aera(width: usize, height: usize)->usize{
    with * height
}

let with = 30;
let height = 40;
println!("{}",aera(width, height));


// tuple 实现
fn aera(rec:(usize, usize))->usize{
    rec.0 * rec.1
}

let rectangle = (30,40);
println!("{}",aera(rectangle))


// struct 实现
struct Rectangle{
    width: usize,
    height: usize,
}
fn aera(rectangle: &Rectangle)->usize{
    rectangle.width * rectangle.height
}

let rect = Rectangle{width: 30, height: 40};
println!("{}", aera(&rect));
