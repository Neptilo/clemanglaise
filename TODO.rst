Priority 1
----------
* Transfer the actions of this file into Bitbucket's bug tracker as improvements, then delete this file

Priority 2
----------
* Adapt the app for a mobile version

  * Handwriting area size: make it fill all available space
  * Update handwriting_layout and vertical_layout on device orientation change
  * Solve bug that makes some Chinese characters not show correctly since Android L

* Improve graphical interface

  * Increase font size of user text input
  * Increase font size of the word being asked
  * Apply the same style to other views (including increasing icon sizes)
  * Reorganize attributes: first data attributes, second GUI attributes, third action attributes
  * Add scrollbars where widgets are bigger than window (harder than it seems)
  * Edit combo box delegate so that the mouse release event checks a box only if it received a mouse press before

Priority 3
----------
* Don't care for spaces in correct pronunciation of an asked word (standardize it in a form that doesn't contain spaces)
* When editing a word:

  * If it succeeds, go back to previous view which will display the success message.
  * If it fails, re-enable input fields to allow the user to change the incorrect fields.

* Duplicates:

  * When choosing a best duplicate from a generated duplicate list, take into account not only word but also part of speech and hint
  * Apply this duplicate check when adding a new word (factor a maximum of code that has already been written for imports)
  * When generating the list of duplicate candidates, possibly take into account part of speech or hint, taking efficiency into consideration
    
* Implement the button to delete a word
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
  * Limit search results to 32 (for example) and inform the user about it. For example write in the status label: "The search results are limited to 32. Please consider narrowing you search to get more specific results."
  
* In online search results, provide a button per entry to import it
* Publicly release the app with an installer
* In multiple import, implement behavior to ask for every item where there is a conflict. In this case the user should be prompted for the behavior for every detected duplicate grouped at the end of the import process.
