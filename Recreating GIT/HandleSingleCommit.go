package main

type Node interface {
	//if it is removed Node, then meta[1] and data will be null
	//If addded then meta[0] will be null
	//Too much unecessary space?
	//Can we create two nodes?
	//implement time stamp
	metaA string
	metaR string
	data  string

	removed *Node
	added   *Node
}

func main() {
	var loc_file string //fetch file location
	//Creates initial main Commit
	//Calls has been edited and the calls Edit function
	if hasBeenEdited() == true {
		Edit(loc_file)
	}
}

func hasBeenEdited() bool {
	var check bool
	//check if user has saved changes,
	//so write comparision function to check file being saved by a user

	return check
}

func Edit(loc_file string) {
	var changes [3]string
	var location = loc_file
	//track file changes
	changes[0], changes[1], changes[2] = track_fileChanges(location)
	store_change(changes)
}

func store_change(changes [3]string) {
	//will store changes in tree structure
	//perform the task of adding nodes to tree
}

func track_fileChanges(location string) (string, string, string) {
	var MdataAdd string
	var MdataRem string
	var Data string
	//supposed to read file and store 3 components of data
	//1)metaData of removed (location of line to be deleted)
	//2)metaData of added (location of line to be added)
	//3)Data of added (file storing changes made)

	//return above three values as a list of type string
	return MdataAdd, MdataRem, Data
}
