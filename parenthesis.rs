use std::io;

fn pop_matching( stack: &mut Vec<char>, expected_top: char ) -> bool
{
    if stack.is_empty() {
        return true;
    }
    
    return stack.pop().unwrap() != expected_top;
}

fn main() {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let expression = input_line.trim().to_string();

    let mut stack = Vec::new();
    let mut fail = false;
    
    for c in expression.chars() {
        match c {
            '[' | '(' | '{' => stack.push( c ),
            ']' => fail = pop_matching( &mut stack, '[' ),
            ')' => fail = pop_matching( &mut stack, '(' ),
            '}' => fail = pop_matching( &mut stack, '{' ),
            _ => {}
        }

        if fail {
            break;
        }
    }

    if fail || !stack.is_empty() {
        println!( "false" );
    } else {
        println!( "true" );
    }
}
