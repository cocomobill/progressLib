#!/usr/bin/env python3
import unittest
import tempfile
import subprocess
import shutil

class ProgressTest(unittest.TestCase):
    def test_width_0(self):
        for i in range(2):
            result = run_cpp(f'Progress p(0, 100); std::cout << p.toBar({i});')
            self.assertEqual(result, '')

    def test_width_2_empty(self):
        result = run_cpp('Progress p(0, 100); std::cout << p.toBar(2);')
        self.assertEqual(result, '[]')

    def test_width_2_full(self):
        result = run_cpp('Progress p(0, 100); p.update(100); std::cout << p.toBar(2);')
        self.assertEqual(result, '[]')

    def test_width_3_empty(self):
        result = run_cpp('Progress p(0, 100); std::cout << p.toBar(3);')
        self.assertEqual(result, '[ ]')

    def test_width_3_full(self):
        result = run_cpp('Progress p(0, 100); p.update(100); std::cout << p.toBar(3);')
        self.assertEqual(result, '[#]')

    def test_width_4_empty(self):
        result = run_cpp('Progress p(0, 100); std::cout << p.toBar(4);')
        self.assertEqual(result, '[  ]')

    def test_width_4_half(self):
        result = run_cpp('Progress p(0, 100); p.update(50); std::cout << p.toBar(4);')
        self.assertEqual(result, '[# ]')

    def test_width_4_full(self):
        result = run_cpp('Progress p(0, 100); p.update(100); std::cout << p.toBar(4);')
        self.assertEqual(result, '[##]')


# Ugliness

def run_cpp(code: str) -> str:
    with tempfile.TemporaryDirectory() as tempdir:
        shutil.copy('./Progress.h', tempdir)
        shutil.copy('./Progress.cpp', tempdir)
        with open(f'{tempdir}/Progress.h', 'a+') as progress:
                progress.write('''
char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
''')
        
        with open(f'{tempdir}/main.cpp', 'w+') as code_file:
            code_file.write('''
#include <iostream>
#include <cstdio>
#include "Progress.cpp"
int main() {
CODE
}'''.replace('CODE', code))
        #print(open(f'{tempdir}/main.cpp').read())
        subprocess.run(['c++', 'main.cpp', '-O0', '-o', 'main', '-fpermissive'], cwd=tempdir, capture_output=True)
        return subprocess.run(['./main'], capture_output=True, cwd=tempdir).stdout.decode('utf-8')

if __name__ == '__main__':
    unittest.main()
