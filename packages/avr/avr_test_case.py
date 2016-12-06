#
# eChronos Real-Time Operating System
# Copyright (C) 2015  National ICT Australia Limited (NICTA), ABN 62 102 206 173.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, version 3, provided that these additional
# terms apply under section 7:
#
#   No right, title or interest in or to any trade mark, service mark, logo or
#   trade name of of National ICT Australia Limited, ABN 62 102 206 173
#   ("NICTA") or its licensors is granted. Modified versions of the Program
#   must be plainly marked as such, and must not be distributed using
#   "eChronos" as a trade mark or product name, or misrepresented as being the
#   original Program.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# @TAG(NICTA_AGPL)
#
'''
A GdbTestCase that is specifically tailored to Atmel AVR systems.

This class is not implemented in pylib/tests.py to keep it close to the rest of the AVR specific code.
This class is not implemented in packages/avr/test.py because there it would be picked up the unittest framework as
a test case itself.
'''
import subprocess
import sys

import pylib.tests


class AvrTestCase(pylib.tests.GdbTestCase):
    def _get_executable_name(self):
        return 'system'

    def _get_test_command(self):
        return ('avr-gdb', '--batch', '-x', self.gdb_commands_path, self.executable_path)

    def _start_helpers(self):
        self._simulavr_output_file = open(self.executable_path + '_simulavr_output.txt', 'w')
        self._simulavr_popen = subprocess.Popen(('simulavr', '--device', 'atmega128', '--gdbserver'),
                                                stdout=self._simulavr_output_file, stderr=self._simulavr_output_file)

    def _stop_helpers(self):
        if sys.platform == 'win32':
            # On Windows, terminating the simulavr process itself is insufficient.
            # It spawns a child process and killing the parent process does not terminate the child process.
            # Therefore, determine the child processes and terminate them explicitly.
            import psutil  # import here so that all other x.py functionality can be used without psutil
            parent_process = psutil.Process(self._simulavr_popen.pid)
            child_processes = parent_process.children(recursive=True)
            for child_process in child_processes:
               child_process.kill()

        self._simulavr_popen.kill()
        self._simulavr_output_file.close()
