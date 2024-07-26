#/usr/bin/python3

import configparser
import re
import subprocess
import os

ip_pattern = re.compile(
    r'^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.'  # First octet
    r'(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.'  # Second octet
    r'(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.'  # Third octet
    r'(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$',   # Fourth octet
    re.ASCII
)

def validate_ip(ip):
    return bool(ip_pattern.match(ip))

def find_config_files(directory):

    config_files = []

    for filename in os.listdir(directory):
        if filename.endswith('.config'):
            config_files.append(filename)

    return config_files

def modify_IP(IP_CPU, IP_MEMORIA, FILE_PATH):

    config = configparser.ConfigParser()

    with open(FILE_PATH, 'r') as f:
        file_content = '[DEFAULT]\n' + f.read()
    config.read_string(file_content)

    # Update the values
    config.set('DEFAULT', 'IP_CPU', IP_CPU)
    config.set('DEFAULT', 'IP_MEMORIA', IP_MEMORIA)

    # Write the changes back to the file
    with open(FILE_PATH, 'w') as f:
        # Write the content without the default section header
        for key in config['DEFAULT']:
            f.write(f"{key.upper()}={config['DEFAULT'][key]}\n")


def main():

    new_ip_cpu = str(input("IP_CPU="))
    new_ip_memoria = str(input("IP_MEMORIA="))
    print()

    if validate_ip(new_ip_cpu) and validate_ip(new_ip_memoria):
        config_files = find_config_files(".")
        for filename in config_files:
            file_path = "./"+filename
            modify_IP(new_ip_cpu, new_ip_memoria, file_path)
            result = subprocess.run(['cat', file_path], capture_output=True, text=True, check=True)
            print(f"[{filename}]")
            print(result.stdout)
    else:
        print("IP inválida, volvé a intentarlo:")
        main()



if __name__ == '__main__':
    main()
