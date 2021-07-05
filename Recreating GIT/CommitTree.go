package commit

type Node struct {
	//timestamp to be added
	loc_of_org_file string
	removed *Node 
	added *Node
}


func NewCommit {
	//Add Commit to commit tree
	//If data was removed, add into left node
	//Also send this commit to HandleSingleCommit's main function to watch for chnages
}

func AlgoWR {
	//algorithm to return required file by
	//computing all changes using two stacks
	//It's input will be the priginal file commited followed by 
	//the changes made to that commit from HandleSingleCommit
}

