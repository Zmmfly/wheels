fn vec_itr(input: Vec<u8>)
{
    for item in input.iter()
    {
        println!("number: {}, hex: {:#02x}", item, item)
    }
}

fn main()
{
    let v:Vec<u8> = vec![127, 0x55, 0xaa];
    vec_itr(v);
}
