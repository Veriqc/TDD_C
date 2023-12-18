#!/bin/bash

python3 -m pip install --user --upgrade setuptools wheel
python3 -m pip install --user --upgrade setuptools wheel
echo "pacakage satisfy"

python3 setup.py sdist bdist_wheel
twine upload --repository-url https://test.pypi.org/legacy/ dist/* -u __token__ -p pypi-AgENdGVzdC5weXBpLm9yZwIkNzc0ZWFlYWYtYTZjMC00MzgyLThmZmYtNjc1MmM5MTdlZTlkAAIqWzMsImE1MzQzZjY0LWE0YWUtNDI0OC04NGJkLTI0YzM3NTNjY2RlOSJdAAAGIO_a0o974YmU7BMFpbvo_5sc_JApSncMsImzD-vjvjoC --verbose

read -p "Press Enter to exit..."