 # KNN Classifier Server
 A server that allows to classify CSV-formatted data, and can support
several clients at once.
 ## Usage
After connecting to the server, you'll see the following menu:
```text
Welcome to the KNN Classifier Server. Please choose an option:
1. upload test/train csv data files
2. algorithm settings
3. classify data
4. display results
5. download results
6. confusion matrix
7. exit
```
1. upload the testing and training csv files
2. change the algorithm settings; note that there are no default settings, please enter them yourself
3. classify the actual data
4. display the classification results
5. download the classification results to a client-specified path
6. see the [confusion matrix](https://en.wikipedia.org/wiki/Confusion_matrix) of the classifier
7. exit

The server will send an appropriate error message in case of incorrect
user-input or internal server errors.

## Code
The code is split to the client and server folders, with common utils 
in `common.cpp`. 

### Server-side
The classifier logic is found in `server\classifier`,
the csv parsing in `server\csv`, and the commands themselves in `server\commands`.

Each command contains a reference to a `ClientData` structure (see `server\client_data.cpp`),
which contains general client data that needs to be shared between commands.

The actual cli functionality is implemented in `server\cli.cpp`, and
the server's networking-related functionality (binding to address, setting timeout, etc) is found
in `server\server.cpp`.

### Client-side
The client has different command handlers, to communicate with the server
on a per-command basis. These are found in `client\command_handlers`.
# How to Build
We can use CMake:
```shell
cmake --build /path/to/server/executable --target ap1_classifier_server
cmake --build /path/to/client/executable --target ap1_classifier_client
```
# Limitations
* The client can be optimized further using threading (sender/receiver threads, etc)
* The server currently creates a new thread for every client, which isn't resource efficient; a better approach would be to use a thread pool,
or a more elaborate queue-based scheme
* The current implementation of the CSV parser supports only 4 feature columns
and one string class, even though the KNN classifier isn't as restricted; changing this is problematic as
it the current implementation requires the feature size to be known at compile time