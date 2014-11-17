Priority 1
----------
* Adapt the app for a mobile version

  * Handwriting area size: make it fill all available space
  * Make import wizard visible
  * Update handwriting_layout and vertical_layout on device orientation change
  * Increase icon sizes

* Improve graphical interface

  * Prevent the tag list from hiding: a possible solution is to use a QHBoxLayout instead of a QToolBox
  * Increase font size of user text input
  * Increase font size of the word being asked
  * Apply the same style to other views
  * For consistency, define all widget attributes as pointers
  * Reorganize attributes: first data attributes, second GUI attributes, third action attributes
  * Add a combo box to filter by theme in the search view

Priority 2
----------
* Don't care for spaces in correct pronunciation of an asked word (standardize it in a form that doesn't contain spaces)
* Implement the button to delete a word
* Find a way to map language ISO codes to their names
* Create app icon: My idea is to represent a nice chocolate cake with crème anglaise spilling on it, like this: 
  http://www.howdoyousaythatword.com/wp-content/uploads/2012/01/creme-anglaise.jpg, use a vector graphics editor.
* Treat all cases when there is no Internet connection.
* Improve ergonomics: allow to do everything using the keyboard only.

  * An "OK" button should always be pressable with the Enter key or Ctrl+Enter if there is an ambiguity.
  * A "Cancel" or "Go back" button should always be pressable with the Esc key.
  * All the modifiable widgets should be accessible with Tab.
  
* Improve search:

  * Develop a search string as multiple search strings with different functions applied to them: for example in Chinese, develop a search "ma3" into a search for "mǎ", develop a search "ma" into searches for "mā", "má", "mǎ", "mà".
  * Allow searches for multiple strings at a time
  * Highlight matched strings
  
* Publicly release the app with an installer
* Include a field for the gender in German
* In multiple import, implement behavior to ask for every item where there is a conflict. In this case the user should be prompted for the behavior for every detected duplicate grouped at the end of the import process.