use std::fs::File;
use std::io::{self, BufRead, Error, Write};
use std::arch::asm;
use std::env;

fn read_integers_from_file(filename: &str, arr: &mut [i32], num_integers: usize) -> Result<(), Error> {
    let file = File::open(filename)?;

    let reader = io::BufReader::new(file);

    let mut parsed_integers = 0;
    for line in reader.lines() {
        if let Ok(line) = line {
            let integers: Vec<i32> = line
                .split("\n")
                .filter(|s| !s.trim().is_empty()) // Skip empty substrings
                .map(|int_str| int_str.trim().parse::<i32>().expect("Failed to parse integer"))
                .collect();

            for &integer in &integers {
                if parsed_integers >= num_integers {
                    break;
                }
                arr[parsed_integers] = integer;
                parsed_integers += 1;
            }

            if parsed_integers >= num_integers {
                break;
            }
        }
    }

    if parsed_integers < num_integers {
        return Err(Error::new(
            std::io::ErrorKind::UnexpectedEof,
            "File contains fewer integers than expected",
        ));
    }

    Ok(())
}

fn write_array_to_file(filename: &str, arr: &[i32], n: usize) -> io::Result<()> {
    let mut file = File::create(filename)?;

    for (i, &num) in arr.iter().take(n).enumerate() {
        write!(file, "{}", num)?;
        if i != n - 1 {
            write!(file, ",")?;
        }
    }

    Ok(())
}

fn rdtscp() -> u64 {
    let mut lo: u32;
    let mut hi: u32;
    unsafe {
        asm!("rdtscp",
        out("eax") lo,
	out("edx") hi);
    };
    ((hi as u64) << 32) | (lo as u64)
}

fn handle_overflow(start: u64, stop: u64) -> u64 {
    if stop < start {
        (u64::MAX - start) + stop
    } else {
        stop - start
    }
}   

fn sort(arr: &mut [i32], n: usize) {
    let mut temp;
    for i in 0..n-1 {
        for j in 0..n-i-1 {
            if arr[j] > arr[j+1] {
		temp = arr[j];
		arr[j] = arr[j+1];
		arr[j+1] = temp;
            }
        }
    }
}

fn main() -> io::Result<()> {
    let input_filename = "../input_array.txt";
    let output_filename = "../output_array.txt";
    let start;
    let stop;
    let args: Vec<_> = env::args().collect();
    let n = *(&args[1].parse::<usize>().unwrap());
    let mut arr = vec![0; n];

    read_integers_from_file(input_filename, &mut arr, n)?;    // read n numbers from the input file to the array

    start = rdtscp();                                           // get the clock count form before sort

    sort(&mut arr, n);                                           // sort the array

    stop = rdtscp();                                            // get the clock count from after the sort

    write_array_to_file(output_filename, &arr, n)?;           // write the sorted array to the output file

    let elapsed_cycles = handle_overflow(start, stop);          // return the elapsed counts, with a possible overflow handled
    println!("{}", elapsed_cycles);

    Ok(())
}
