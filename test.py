#!/usr/bin/env python3
import unittest
import tempfile
import subprocess
import shutil


class ProgressTest(unittest.TestCase):
    def test_width_0(self):
        for i in range(2):
            self.assertCpp(f"Progress p(0, 100); std::cout << p.toBar({i});", "")

    def test_width_2_empty(self):
        self.assertCpp("Progress p(0, 100); std::cout << p.toBar(2);", "[]")

    def test_width_2_full(self):
        self.assertCpp(
            "Progress p(0, 100); p.update(100); std::cout << p.toBar(2);", "[]"
        )

    def test_width_3_empty(self):
        self.assertCpp("Progress p(0, 100); std::cout << p.toBar(3);", "[ ]")

    def test_width_3_full(self):
        self.assertCpp(
            "Progress p(0, 100); p.update(100); std::cout << p.toBar(3);", "[#]"
        )

    def test_width_4_empty(self):
        self.assertCpp("Progress p(0, 100); std::cout << p.toBar(4);", "[  ]")

    def test_width_4_half(self):
        self.assertCpp(
            "Progress p(0, 100); p.update(50); std::cout << p.toBar(4);", "[# ]"
        )

    def test_width_4_full(self):
        self.assertCpp(
            "Progress p(0, 100); p.update(100); std::cout << p.toBar(4);", "[##]"
        )

    def test_percentage_passthrough(self):
        for i in [0, 25, 50, 100]:
            self.assertCpp(
                f"Progress p(0, 100); p.update({i}); std::cout << p.toPercentage(4);",
                str(i),
            )

    def test_percentage_fractional(self):
        for i in [0.25, 25.25, 50.25, 99.25]:
            self.assertCpp(
                f"Progress p(0, 100); p.update({i}); std::cout << p.toPercentage(4);",
                str(i),
            )

    def test_padded_percentage(self):
        self.assertCpp(
            "Progress p(0, 100); p.update(10); std::cout << p.toPaddedPercentage(2);",
            " 10.00",
        )

        self.assertCpp(
            "Progress p(0, 1000); p.update(100); std::cout << p.toPaddedPercentage(2);",
            " 10.00",
        )

        self.assertCpp(
            "Progress p(0, 1000); p.update(505); std::cout << p.toPaddedPercentage(2);",
            " 50.50",
        )

        self.assertCpp(
            "Progress p(0, 1000); p.update(1000); std::cout << p.toPaddedPercentage(2);",
            "100.00",
        )

    def assertCpp(self, code, expected):
        result = run_cpp(code)
        self.assertEqual(result, expected)


# Ugliness


def run_cpp(code: str) -> str:
    with tempfile.TemporaryDirectory() as tempdir:
        shutil.copy("./Progress.h", tempdir)
        shutil.copy("./Progress.cpp", tempdir)
        with open(f"{tempdir}/Progress.h", "a+") as progress:
            progress.write(
                """
char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
"""
            )

        with open(f"{tempdir}/main.cpp", "w+") as code_file:
            code_file.write(
                """
                #include <iostream>
                #include <cstdio>
                #include "Progress.cpp"
                int main() {
                CODE
                }""".replace(
                    "CODE", code
                )
            )
        # print(open(f'{tempdir}/main.cpp').read())
        subprocess.run(
            ["c++", "main.cpp", "-O0", "-o", "main", "-fpermissive"],
            cwd=tempdir,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        return subprocess.run(
            ["./main"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=tempdir
        ).stdout.decode("utf-8")


if __name__ == "__main__":
    unittest.main()
