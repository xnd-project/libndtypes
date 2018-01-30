cd "%RECIPE_DIR%\..\..\" || exit 1
"%PYTHON%" setup.py module || exit 1
"%PYTHON%" setup.py module_install || exit 1
if not exist "%RECIPE_DIR%\test" mkdir "%RECIPE_DIR%\test" || exit 1
copy /y python\*.py "%RECIPE_DIR%\test"
