fn u8ref(input: &[u8])
{
    for item in input.iter()
    {
        println!("number: {}, hex: {:#02x}", item, item)
    }
}

fn main()
{
    let v:Vec<u8> = vec![127, 0x55, 0xaa];
    let u8r: &[u8] = &v;
    u8ref(u8r);
}
