import os
import subprocess
import platform

test_files = {
    "stringlist-test.c":["../stringlist.c"],
    "file-test.c":["../stringlist.c", "../file.c"]
}
test_count = len(test_files)

def read_expectedoutput(filename):
    with open(f"expected_outputs/{filename.replace('.c', '.txt')}", "r") as f:
        expected_output = f.read()
    return expected_output

def test_passed(filename):
    with open("output.txt", "r") as f:
        output = f.read()
    expected_output = read_expectedoutput(filename)
    return expected_output == output

for index, test_file in enumerate(test_files):
    print(f"Running test {index+1}/{test_count}: {test_file}")
    link_files = " ".join(test_files[test_file])
    output_file = test_file.replace(".c", "")
    passed = False
    os.system(f"gcc {test_file} {link_files} -o {output_file}")
    if platform.system() == "Windows":
        subprocess.run(f"{output_file}", capture_output=True)
        passed = test_passed(test_file)
    else:
        process = subprocess.run(f"./{output_file}", capture_output=True)
        passed = test_passed(test_file)
    print("Successfully passed test!") if passed else print("Failed test")
os.remove("output.txt")