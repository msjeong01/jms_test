import csv
import re

def process_csv(input_file, output_file):
    with open(input_file, 'r') as f:
        reader = csv.reader(f, delimiter='\t')
        header = next(reader)  # 헤더
        data = list(reader)    # 데이터 리스트 읽기

    new_data = []
    for row in data:
        new_row = []
        for item in row:
            item = re.sub(r'[\[\]\"]', '', item)
            numbers = re.findall(r'[-+]?\d*\.\d+|\d+', item)
            new_row.extend(numbers)
        new_data.append(new_row)

    with open(output_file, 'w', newline='') as f:
        writer = csv.writer(f, delimiter='\t')
        writer.writerow(header)
        writer.writerows(new_data)

input_file = '/home/syscon/Downloads/data0.csv'
output_file = '/home/syscon/test_repo/pose_result/data0(0.05).csv'

process_csv(input_file, output_file)
print("print Done")