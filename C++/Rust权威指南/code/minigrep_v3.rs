use std::env;
use std::fs;
use std::process;
fn main() {
    let args:Vec<String> = env::args().collect();
    let config = Config::new(&args).unwrap_or_else(
        |err|{
            println!("problem phasing arguments :{}",err);
            process::exit(1);
        }
    );
    let content = fs::read_to_string(config.filename).expect("something wrong when reading file");
}


struct Config{
    query: String,
    filename: String,
}
impl Config {
    fn new(args:&[String])->Result<Config, &'static str> {
        if args.len() < 3{
           return Err("not enough arguments");
        }
        let query = &args[1];
        let filename = &args[2];
        Ok(Config{query:query.to_string(),filename:filename.to_string()})
    }
}