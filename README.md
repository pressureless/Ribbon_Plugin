# A ribbon plugin

## Build

Open Ribbon.sln with Microsoft Visual Studio 2010 and build the solution.
You may need to change the configuration, just follow the plugin development document

## How to use

The plugin displays a new Ribbon category with a button called "CloseSpecificFiles", when the user clicks the button, the plugin will check the number of files. If there's no file, then it will show a message box, otherwise it will display the file information. The user can select specific files to close.

Each file contains six information items: Filename, Directory, Reopen time, Duration, Modified and Pages.

## Credits

ScrollView is modified from online examples, thanks to the author
