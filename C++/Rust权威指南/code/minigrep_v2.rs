use std::env;
use std::fs;
fn main() {
    let args:Vec<String> = env::args().collect();
    let config = Config::new(&args);
    let content = fs::read_to_string(config.filename).expect("something wrong when reading file");
}


struct Config{
    query: String,
    filename: String,
}
impl Config {
    fn new(args:&[String])->Config{
        let query = &args[1];
        let filename = &args[2];
        Config{query:query.to_string(),filename:filename.to_string()}
    }
}