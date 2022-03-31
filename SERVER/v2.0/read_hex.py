


def check_hex_file(records):
    error_flag = 0 # check on bad records
    line_counter =1 # to know the current line in the file
    for record in records:
        checksum = 0 # needs to be zero at the beginning of calculating CheckSum for each field
        for i in range(0, len(record) - 4, 2): # loob to sum the record except the checksum byte
            checksum += int(record[i:i + 2], 16)

        checksum &= 0xFF # need least byte only
        value = (checksum + int(record[-3: -1], 16)) & 0xFF # in case tof an overflow in the sum

        if value != 0:
            print("corrupted hex record at line ",line_counter)
            error_flag =1

        line_counter+=1

    if error_flag:
        return 1
    else:
        return 0



