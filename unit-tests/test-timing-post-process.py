class Timing:
    def __init__(self, time: float, test_name: str):
        self.time = time
        self.test_name: str = test_name
        self.difference = -1
    
    def __eq__(self, other: "Timing"):
        return self.test_name.lower() == other.test_name.lower()
    
    def __sub__(self, other: "Timing"):
        return self.time - other.time

    def to_string(self):
        return f"{self.test_name}: {self.time} s"
    
    def to_string_w_diff(self):
        diff = self.difference
        if diff == -1:
            diff = 0
        return f"{self.test_name}: {round(self.time,3)} s, Difference: {round(diff,3)}"
    
def create_timing_from_test_line(line: str) -> Timing:
    time_end_idx: int = line.find(" ")
    if time_end_idx == -1:
        return None
    try:
        time: float = float(line[0:time_end_idx])
    except:
        print("Couldn't convert: " + line[0:time_end_idx] + " to a float")
        return None
    
    test_name = line[time_end_idx+4:-1]
    return Timing(time, test_name)

def parse_test_file(file_path: str) -> list[Timing]:
    timings: list[Timing] = []
    with open(file_path, 'r') as file:
        previous_line = ""
        for line in file:
            if line.find("Timing Tests") != -1:
                timing = create_timing_from_test_line(previous_line)
                if timing is not None:
                    timings.append(timing)
            previous_line = line[:] # deep copy line
    return timings

def parse_timing_file(file_path: str) -> list[Timing]:
    timings: list[Timing] = []
    with open(file_path, 'r') as file:
        for line in file:
            seperator_idx = line.find(":")
            if seperator_idx == -1:
                continue
            test_name = line[:seperator_idx]
            try:
                time = float(line[seperator_idx+2:-2])
                print(time)
            except:
                print("Couldn't convert: " + line[seperator_idx:-2] + " to a float")
                continue
            timings.append(Timing(time, test_name))


    return timings

def save_timings(timings: list[Timing], file_path: str):
    with open(file_path, 'w') as file:
        for timing in timings:
            file.write(f"{timing.to_string()}\n")

parse_file_path = "matrix-test-timings-temp.txt"
save_file_path = "matrix-test-timings.txt"

# get the new timings
new_timings = parse_test_file(parse_file_path)

# get the old timings
old_timings = parse_timing_file(save_file_path)

difference_increased = ""
# calculate the timing difference
for new_timing in new_timings:
    for old_timing in old_timings:
        if new_timing == old_timing:
            new_timing.difference = new_timing - old_timing
            if abs(new_timing.difference) >= 0.03:
                difference_increased += f"{new_timing.test_name}, "

def save_option():
    # save the new timings
    while True:
        option = input("Save Results? (y/n)")
        if option[0].lower() == 'y':
            save_timings(new_timings, save_file_path)
            print("Saved.")
            break
        elif option[0].lower() == 'n':
            break

# print the new timing results along with the difference
for timing in new_timings:
    print(timing.to_string_w_diff())

if len(difference_increased) > 0:
    print("You've made major timing changes for:" + difference_increased)
    save_option()
else:
    print("No times have changed outside the margin of error.")
