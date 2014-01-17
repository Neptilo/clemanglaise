* Fix this bug: on online tests (I don't know about offline), when you add a theme which the database assigns an ID that is not equal to (the last ID+1), and you go back to the test to select the new theme, it crashes.
* Create app icon: My idea is to represent a nice chocolate cake with crème anglaise spilling on it, like this: 
  http://www.howdoyousaythatword.com/wp-content/uploads/2012/01/creme-anglaise.jpg, use a vector graphics editor.
* Treat all cases when there is no Internet connection.
* Change font sizes, especially the word being asked
* Set a default window width (?)
* Improve ergonomy: allow to do everything using the keyboard only.

  * An "OK" button should always be pressable with the Enter key or Ctrl+Enter if there is an ambiguity.
  * A "Cancel" or "Go back" button should always be pressable with the Esc key.
  * All the modifiable widgets should be accessible with Tab.
  
* Improve search:

  * Develop a search string as multiple search strings with different functions applied to them: for example in Chinese, develop a search "ma3" into a search for "mǎ", develop a search "ma" into searches for "mā", "má", "mǎ", "mà".
  * Allow searches for multiple strings at a time
  * Highlight matched strings
  
* Implement new method for calculating scores:
  Instead of increasing or descreasing a score when an answer is right or wrong, update two variables in different fields of the table: increase the value of a field ``correctly_answered`` if the question was answered correctly, increase the value of a field ``asked`` in both cases, and update a ``score`` field as correctly_answered/(asked+1). The app will then still ask for the words with the lowest scores.
  Then the words asked are no longer the ones that have been reviewed a long time ago, but the ones that are answered the most frequently wrong.
* Publicly release the app with an installer
* Include a field for the gender in German
* Being able to import an online vocabulary list, or a sublist, or a single word. In each case, a new view will appear asking the user if they want to make it a new list, or import it into an existing list. In the latter case, check if the `word` field of the inserted items don't already exist in the existing list. Let the user choose to:

  * only keep the already existing item if there is a conflict
  * write over existing items
  * merge them, that is, for a ``meaning`` field "x" offline, and "y" online, set it to "x, y"
  * ask for every item where there is a conflict
  * (edit manually?)
  
* Add a combo box to filter by theme in the search view