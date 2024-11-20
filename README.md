# nyu-application-security-hw1

# Application Security Assignment: Beware of Geeks Bearing Gift Cards

## Overview
This project involves auditing and improving a buggy C program designed to read and display gift card files. The task includes identifying vulnerabilities, writing test cases, fixing bugs, and improving coverage using fuzzing tools.

---

## Tasks

### Part 1: Setting Up the Environment
1. **Git Configuration:**
   - Install and configure Git.
   - Use signed commits.
   - Create a GitHub repository and set it to private.

2. **Travis CI Setup:**
   - Link GitHub with Travis CI.
   - Create a `.travis.yml` file for C projects.
   - Enable CI/CD for automated builds.

---

### Part 2: Auditing and Test Cases
1. **Bug Identification:**
   - Analyze `giftcardreader.c` for vulnerabilities.
   - Write test cases to expose bugs.

2. **Test Cases:**
   - `crash1.gft` and `crash2.gft`: Two test cases that crash the program due to different root causes.
   - `hang.gft`: A test case causing an infinite loop.

3. **Fixes and Validation:**
   - Fix bugs triggered by the test cases.
   - Automate regression testing using Travis CI.

4. **Documentation:**
   - Add a `bugs.txt` file explaining the issues triggered by the test cases.

---

### Part 3: Fuzzing and Coverage
1. **Coverage Analysis:**
   - Measure initial coverage using `lcov`.
   - Write two new test cases (`cov1.gft`, `cov2.gft`) to cover uncovered code.

2. **Fuzz Testing:**
   - Use AFL to generate test cases for additional coverage.
   - Run the fuzzer for at least two hours.
   - Add the generated tests to the suite.

3. **Bug Fixes:**
   - Fix two unique bugs found by the fuzzer.
   - Re-run all tests to ensure fixes are effective.

4. **Updated Tests:**
   - Use `afl-tmin` to minimize redundant tests.
   - Automate testing of all new cases with Travis CI.

5. **Documentation:**
   - Create a `writeup.txt` summarizing the bugs found and fixed in this phase.

---

## Grading Breakdown
- **Part 1 (20 points):**  
  - Signed commits (10 points).  
  - Travis CI configuration (10 points).  

- **Part 2 (40 points):**  
  - Test cases and fixes (10 points).  
  - Bug writeup (10 points).  
  - Regression testing with Travis CI (10 points).  

- **Part 3 (40 points):**  
  - Handwritten tests (10 points).  
  - Fuzzer-generated tests (10 points).  
  - Code fixes (10 points).  
  - Final writeup (10 points).  

---

## Submission
- Provide a GitHub repository link with the following structure:
  - **Part 1:** `.travis.yml`, signed commits.
  - **Part 2:** Directory `part2` with test files (`crash1.gft`, `crash2.gft`, `hang.gft`) and `writeup.txt`.
  - **Part 3:** Directory `part3` with test files (`cov1.gft`, `cov2.gft`, `fuzzer1.gft`, `fuzzer2.gft`) and `writeup.txt`.

- Add the instructor (`JustinCappos`) and TA (`Yash151`) as collaborators on GitHub.

---

## Notes
- Despite fixes, the program may still have bugs. A complete rewrite in a safer language like Python or Rust is recommended for robust security.
- A bug-free version of the program in C will be released for reference post-assignment.

---
