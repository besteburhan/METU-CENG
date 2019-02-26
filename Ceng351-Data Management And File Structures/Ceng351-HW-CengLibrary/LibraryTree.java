import java.util.ArrayList;
public class LibraryTree {

	public LibraryNode primaryRoot;		//root of the primary B+ tree
	public LibraryNode secondaryRoot;	//root of the secondary B+ tree
	public LibraryTree(Integer order)
	{
		LibraryNode.order = order;
		primaryRoot = new LibraryNodeLeaf(null);
		primaryRoot.level = 0;
		secondaryRoot = new LibraryNodeLeaf(null);
		secondaryRoot.level = 0;
	}


	public void addBook(CengBook book)
	{
			//primary b+tree
		// Secondary b+tree
		primary(book);
		secondary(book);

	}

	private void primary(CengBook book){

				int order = LibraryNode.order;
				LibraryNodePrimaryIndex src =new LibraryNodePrimaryIndex(null);
				LibraryNodeLeaf srcLeaf = new LibraryNodeLeaf(null);

				int key = book.key();
				//go to the leaf
				if(primaryRoot.getType()==LibraryNodeType.Internal){

						src=(LibraryNodePrimaryIndex) primaryRoot;
						while(srcLeaf.getParent()==null){
							for(int i=0; i < src.keyCount() ; i++){
								if(key < src.keyAtIndex(i)){
									if(src.getChildrenAt(i).getType()==LibraryNodeType.Internal){
										src = (LibraryNodePrimaryIndex)src.getChildrenAt(i);
										break;
									}
									else {
										srcLeaf = (LibraryNodeLeaf)src.getChildrenAt(i);
										break;
									}
								}
								else if(i==src.keyCount()-1){
									if(src.getChildrenAt(i+1).getType()==LibraryNodeType.Internal){
										src = (LibraryNodePrimaryIndex)src.getChildrenAt(i+1);
										break;
									}
									else{
										srcLeaf = (LibraryNodeLeaf)src.getChildrenAt(i+1);
										break;
									}
								}
							}
						}
					}
					else if(srcLeaf.getParent()==null)
						srcLeaf=(LibraryNodeLeaf) primaryRoot;
				//add to leaf
				if(srcLeaf.bookCount()==0) srcLeaf.addBook(0,book);
				else{

						for(int i=0;i<srcLeaf.bookCount(); i++){
							if(key < srcLeaf.bookKeyAtIndex(i)){
								srcLeaf.addBook(i,book);
								break;
							}
							else if(i==srcLeaf.bookCount()-1){
								srcLeaf.addBook(i+1,book);
								break;
							}
						}
					}
				//if there is no overflow, there is nothing to do
				//what if overflow happens? copy_up
				if(srcLeaf.bookCount()>2*order){// dividing leaf,key copied to parent
							ArrayList<LibraryNode> splitted_leaves = split_leaf(srcLeaf);
							LibraryNodeLeaf leaf_first = (LibraryNodeLeaf) splitted_leaves.get(0);
							LibraryNodeLeaf leaf_second = (LibraryNodeLeaf) splitted_leaves.get(1);
							int middle_key = leaf_second.bookKeyAtIndex(0);
							LibraryNodePrimaryIndex parent = (LibraryNodePrimaryIndex) srcLeaf.getParent();
							int insert_children=0;
							if(parent==null){
									LibraryNodePrimaryIndex root = new  LibraryNodePrimaryIndex(null);
									root.addKey(0,middle_key);
									leaf_first.setParent(root);
									leaf_second.setParent(root);
									root.addChildren(0,leaf_first);
									root.addChildren(1,leaf_second);
									primaryRoot = root;
							}
							else
									{
										for(int i=0; i<parent.keyCount();i++){
											if(middle_key < parent.keyAtIndex(i)){
												parent.addKey(i,middle_key);
												insert_children=i;
												break;
											}
											else if(i==parent.keyCount()-1){
												parent.addKey(i+1,middle_key);
												insert_children=i+1;
												break;
											}
										}
										LibraryNode root = call_recursive_Primary(primaryRoot,parent,insert_children,leaf_first,leaf_second);
										primaryRoot = root;
									}
				}
	}
	private void secondary(CengBook book){
		int order = LibraryNode.order;
		LibraryNodeSecondaryIndex srcSec =new LibraryNodeSecondaryIndex(null);
		LibraryNodeLeaf srcSecLeaf = new LibraryNodeLeaf(null);
		int key = book.key();
		int year = book.year();
		//go to the leaf
		if(secondaryRoot.getType()==LibraryNodeType.Internal){

				srcSec=(LibraryNodeSecondaryIndex) secondaryRoot;
				while(srcSecLeaf.getParent()==null){
					for(int i=0; i < srcSec.keyCount() ; i++){

						if(year< srcSec.yearAtIndex(i) ||
						(year == srcSec.yearAtIndex(i) && key < srcSec.keyAtIndex(i))){
							if(srcSec.getChildrenAt(i).getType()==LibraryNodeType.Internal){
								srcSec = (LibraryNodeSecondaryIndex)srcSec.getChildrenAt(i);
								break;
							}
							else {
								srcSecLeaf = (LibraryNodeLeaf)srcSec.getChildrenAt(i);
								break;
							}
						}
						else if(i==srcSec.keyCount()-1){
							if(srcSec.getChildrenAt(i+1).getType()==LibraryNodeType.Internal){
								srcSec = (LibraryNodeSecondaryIndex)srcSec.getChildrenAt(i+1);
								break;
							}
							else{
								srcSecLeaf = (LibraryNodeLeaf)srcSec.getChildrenAt(i+1);
								break;
							}
						}
					}
				}
			}
			else if(srcSecLeaf.getParent()==null)
				srcSecLeaf=(LibraryNodeLeaf) secondaryRoot;
		//add to leaf
		if(srcSecLeaf.bookCount()==0) srcSecLeaf.addBook(0,book);
		else{

				for(int i=0;i<srcSecLeaf.bookCount(); i++){
					if(year<srcSecLeaf.bookYearAtIndex(i) || (year == srcSecLeaf.bookYearAtIndex(i) && key < srcSecLeaf.bookKeyAtIndex(i))){
						srcSecLeaf.addBook(i,book);
						break;
					}
					else if(i==srcSecLeaf.bookCount()-1){
						srcSecLeaf.addBook(i+1,book);
						break;
					}
				}
			}
		//if there is no overflow, there is nothing to do
		//what if overflow happens? copy_up
		if(srcSecLeaf.bookCount()>2*order){// dividing leaf,key copied to parent
					ArrayList<LibraryNode> splitted_leaves = split_leaf(srcSecLeaf);
					LibraryNodeLeaf leaf_first = (LibraryNodeLeaf) splitted_leaves.get(0);
					LibraryNodeLeaf leaf_second = (LibraryNodeLeaf) splitted_leaves.get(1);
					int middle_key = leaf_second.bookKeyAtIndex(0);
					int middle_year = leaf_second.bookYearAtIndex(0);
					LibraryNodeSecondaryIndex parent = (LibraryNodeSecondaryIndex) srcSecLeaf.getParent();
					int insert_children=0;
					if(parent==null){
							LibraryNodeSecondaryIndex root = new  LibraryNodeSecondaryIndex(null);
							root.addYearKey(0,middle_year, middle_key);
							leaf_first.setParent(root);
							leaf_second.setParent(root);
							root.addChildren(0,leaf_first);
							root.addChildren(1,leaf_second);
							secondaryRoot = root;
					}
					else
							{
								for(int i=0; i<parent.keyCount();i++){
									if(middle_year < parent.yearAtIndex(i) ||
									 	(middle_year == parent.yearAtIndex(i) && middle_key < parent.keyAtIndex(i))){
											insert_children=i;
										parent.addYearKey(i, middle_year, middle_key);
										break;
									}
									else if(i == parent.keyCount() - 1){
										parent.addYearKey(i + 1, middle_year, middle_key);
										insert_children = i + 1;
										break;
									}
								}
								LibraryNode root = call_recursive_Secondary(secondaryRoot,parent,insert_children,leaf_first,leaf_second);
								secondaryRoot = root;
							}
		}
	}
	private LibraryNode call_recursive_Secondary(LibraryNode secondaryRoot,LibraryNode src,int insert_children,LibraryNode first_node, LibraryNode second_node){
					int order = LibraryNode.order;
					LibraryNodeSecondaryIndex parent = new LibraryNodeSecondaryIndex(null);
					LibraryNodeSecondaryIndex first_int= new LibraryNodeSecondaryIndex(null);
					LibraryNodeSecondaryIndex second_int= new LibraryNodeSecondaryIndex(null);
					parent = (LibraryNodeSecondaryIndex) src;
					LibraryNodeSecondaryIndex root = new  LibraryNodeSecondaryIndex(null);
					if(parent.keyCount()<=2*order)//done there is no push up
					{
								parent.removeChildren(insert_children);
								first_node.setParent(parent);
								second_node.setParent(parent);
								parent.addChildren(insert_children,first_node);
								parent.addChildren(insert_children+1,second_node);
								return secondaryRoot;
					}
					else if(parent.keyCount()>2*order)//there is push up operation
					{
								int push_up_key = parent.keyAtIndex(order);
								int push_up_year = parent.yearAtIndex(order);
								ArrayList<LibraryNode> splitted_internals = split_internal_sec(parent,insert_children,first_node,second_node);
								first_int =(LibraryNodeSecondaryIndex) splitted_internals.get(0);
								second_int = (LibraryNodeSecondaryIndex)splitted_internals.get(1);
								if(parent.getParent()==null){// create a new root,then done ->root can have 1 key

									root.addYearKey(0, push_up_year, push_up_key);
									first_int.setParent(root);
									second_int.setParent(root);
									root.addChildren(0,first_int);
									root.addChildren(1,second_int);
									return root;
								}
								else{
										parent = (LibraryNodeSecondaryIndex)parent.getParent();
										first_int.setParent(parent);
										second_int.setParent(parent);

										for(int i=0; i<parent.keyCount();i++){
											if( push_up_year < parent.yearAtIndex(i) ||
												(push_up_year == parent.yearAtIndex(i) &&	push_up_key < parent.keyAtIndex(i))){
												parent.addYearKey(i, push_up_year, push_up_key);
												insert_children=i;
												break;
											}
											else if(i==parent.keyCount() - 1){
												parent.addYearKey(i + 1, push_up_year, push_up_key);
												insert_children = i + 1;
												break;
											}
										}
										root = (LibraryNodeSecondaryIndex)call_recursive_Secondary(secondaryRoot,parent,insert_children,first_int,second_int);
								}
						}
						return root;
		}
	private LibraryNode call_recursive_Primary(LibraryNode primaryRoot,LibraryNode src,int insert_children,LibraryNode first_node, LibraryNode second_node){
				int order = LibraryNode.order;
				LibraryNodePrimaryIndex parent = new LibraryNodePrimaryIndex(null);
				LibraryNodePrimaryIndex first_int= new LibraryNodePrimaryIndex(null);
				LibraryNodePrimaryIndex second_int= new LibraryNodePrimaryIndex(null);
				parent = (LibraryNodePrimaryIndex) src;
				LibraryNodePrimaryIndex root = new  LibraryNodePrimaryIndex(null);
				if(parent.keyCount()<=2*order)//done there is no push up
				{
							parent.removeChildren(insert_children);
							first_node.setParent(parent);
							second_node.setParent(parent);
							parent.addChildren(insert_children,first_node);
							parent.addChildren(insert_children+1,second_node);
							return primaryRoot;
				}
				else if(parent.keyCount()>2*order)//there is push up operation
				{
							int push_up_key = parent.keyAtIndex(order);
							ArrayList<LibraryNode> splitted_internals = split_internal(parent,insert_children,first_node,second_node);
							first_int =(LibraryNodePrimaryIndex) splitted_internals.get(0);
							second_int = (LibraryNodePrimaryIndex)splitted_internals.get(1);
							if(parent.getParent()==null){// create a new root,then done ->root can have 1 key

								root.addKey(0,push_up_key);
								first_int.setParent(root);
								second_int.setParent(root);
								root.addChildren(0,first_int);
								root.addChildren(1,second_int);
								return root;
							}
							else{
									parent = (LibraryNodePrimaryIndex)parent.getParent();
									first_int.setParent(parent);
									second_int.setParent(parent);

									for(int i=0; i<parent.keyCount();i++){
										if(push_up_key < parent.keyAtIndex(i)){
											parent.addKey(i,push_up_key);
											insert_children=i;
											break;
										}
										else if(i==parent.keyCount()-1){
											parent.addKey(i+1,push_up_key);
											insert_children=i+1;
											break;
										}
									}
									root = (LibraryNodePrimaryIndex)call_recursive_Primary(primaryRoot,parent,insert_children,first_int,second_int);
							}
					}
					return root;
	}
	private ArrayList<LibraryNode> split_leaf(LibraryNode leaf){
		LibraryNodeLeaf src = (LibraryNodeLeaf) leaf;
			int order = LibraryNode.order;
			ArrayList<CengBook> arr_first = new ArrayList<CengBook>();
			ArrayList<CengBook> arr_second = new ArrayList<CengBook>();
			for(int i=0; i<src.bookCount() ; i++){
				if(i<order)
					arr_first.add(src.bookAtIndex(i));
				else
					arr_second.add(src.bookAtIndex(i));
			}
			LibraryNode leaf_first = new LibraryNodeLeaf(null,arr_first);
			LibraryNode leaf_second = new LibraryNodeLeaf(null,arr_second);
			ArrayList<LibraryNode> result = new ArrayList<LibraryNode>();
			result.add(leaf_first);
			result.add(leaf_second);
			return result;
	}
	private ArrayList<LibraryNode> split_internal(LibraryNode internal, int insert_children, LibraryNode leaf_first,LibraryNode leaf_second){
		LibraryNodePrimaryIndex src = (LibraryNodePrimaryIndex) internal;
		int order = LibraryNode.order;
		ArrayList<Integer> arr_key_first = new ArrayList<Integer>();
		ArrayList<Integer> arr_key_second = new ArrayList<Integer>();
		ArrayList<LibraryNode> arr_children_first = new ArrayList<LibraryNode>();
		ArrayList<LibraryNode> arr_children_second = new ArrayList<LibraryNode>();
		boolean temp1=true;
		boolean temp2=true;
		for(int i=0; i<= 2*order;i++){
			if(i<order){
				arr_key_first.add(src.keyAtIndex(i));
			}
			else if(i>order){
				arr_key_second.add(src.keyAtIndex(i));
			}
		}
		int srcInd =0;
		for(int i=0;i<2*order+2;i++){

			if(i<=order && i<insert_children){
				arr_children_first.add(src.getChildrenAt(srcInd));
				srcInd++;

			}
			else if(i<=order && i==insert_children){
				arr_children_first.add(leaf_first);
				temp1=true;

			}
			else if(i<=order && i==insert_children+1){
				arr_children_first.add(leaf_second);
				srcInd++;
				temp2=true;

			}
			else if(i<=order && i>insert_children+1){
				arr_children_first.add(src.getChildrenAt(srcInd));
				srcInd++;

			}
			else if(i>order && i<insert_children){
				arr_children_second.add(src.getChildrenAt(srcInd));
				srcInd++;
			}
			else if(i>order && i>insert_children+1){
				arr_children_second.add(src.getChildrenAt(srcInd));
				srcInd++;

			}
			else if(i>order && i== insert_children){
				arr_children_second.add(leaf_first);

				temp1=false;

			}
			else if(i>order && i==insert_children+1){
				arr_children_second.add(leaf_second);
				srcInd++;
				temp2=false;
			}
		}
		LibraryNode first =new  LibraryNodePrimaryIndex(null, arr_key_first,arr_children_first);
		LibraryNode second =new  LibraryNodePrimaryIndex(null, arr_key_second,arr_children_second);
		for(LibraryNode child: ((LibraryNodePrimaryIndex) first).getAllChildren()){
				child.setParent(first);
		}
		for(LibraryNode child: ((LibraryNodePrimaryIndex) second).getAllChildren()){
				child.setParent(second);
		}
		ArrayList<LibraryNode> result =new ArrayList<LibraryNode>();
		if(temp1)
			leaf_first.setParent(first);
		else
			leaf_first.setParent(second);
		if(temp2)
			leaf_second.setParent(first);
		else
			leaf_second.setParent(second);
		result.add(first);
		result.add(second);
		return result;
	}
	private ArrayList<LibraryNode> split_internal_sec(LibraryNode internal, int insert_children, LibraryNode leaf_first,LibraryNode leaf_second){
		LibraryNodeSecondaryIndex src = (LibraryNodeSecondaryIndex) internal;
		int order = LibraryNode.order;
		ArrayList<Integer> arr_key_first = new ArrayList<Integer>();
		ArrayList<Integer> arr_key_second = new ArrayList<Integer>();
		ArrayList<Integer> arr_year_first = new ArrayList<Integer>();
		ArrayList<Integer> arr_year_second = new ArrayList<Integer>();
		ArrayList<LibraryNode> arr_children_first = new ArrayList<LibraryNode>();
		ArrayList<LibraryNode> arr_children_second = new ArrayList<LibraryNode>();
		boolean temp1=true;
		boolean temp2=true;
		for(int i=0; i<= 2*order;i++){
			if(i<order){
				arr_year_first.add(src.yearAtIndex(i));
				arr_key_first.add(src.keyAtIndex(i));
			}
			else if(i>order){
				arr_year_second.add(src.yearAtIndex(i));
				arr_key_second.add(src.keyAtIndex(i));
			}
		}
		int srcInd =0;
		for(int i=0;i<2*order+2;i++){

			if(i<=order && i<insert_children){
				arr_children_first.add(src.getChildrenAt(srcInd));
				srcInd++;
			}
			else if(i<=order && i==insert_children){
				arr_children_first.add(leaf_first);
				temp1=true;
			}
			else if(i<=order && i==insert_children+1){
				arr_children_first.add(leaf_second);
				srcInd++;
				temp2=true;

			}

			else if(i<=order && i>insert_children+1){
				arr_children_first.add(src.getChildrenAt(srcInd));
				srcInd++;
			}

			else if(i>order && i<insert_children){
				arr_children_second.add(src.getChildrenAt(srcInd));
				srcInd++;

			}
			else if(i>order && i>insert_children+1){
				arr_children_second.add(src.getChildrenAt(srcInd));
				srcInd++;
			}
			else if(i>order && i== insert_children){
				arr_children_second.add(leaf_first);

				temp1=false;

			}
			else if(i>order && i==insert_children+1){
				arr_children_second.add(leaf_second);
				srcInd++;
				temp2=false;
			}
		}
		LibraryNode first =new  LibraryNodeSecondaryIndex(null, arr_year_first, arr_key_first, arr_children_first);
		LibraryNode second =new  LibraryNodeSecondaryIndex(null, arr_year_second, arr_key_second, arr_children_second);
		for(LibraryNode child: ((LibraryNodeSecondaryIndex) first).getAllChildren()){
				child.setParent(first);
		}
		for(LibraryNode child: ((LibraryNodeSecondaryIndex) second).getAllChildren()){
				child.setParent(second);
		}
		ArrayList<LibraryNode> result =new ArrayList<LibraryNode>();
		if(temp1)
			leaf_first.setParent(first);
		else
			leaf_first.setParent(second);
		if(temp2)
			leaf_second.setParent(first);
		else
			leaf_second.setParent(second);
		result.add(first);
		result.add(second);
		return result;
	}

	public CengBook searchBook(Integer key) {

		LibraryNode searcher = primaryRoot;
		while(searcher.type != LibraryNodeType.Leaf){
			ArrayList<Integer> keys = ((LibraryNodePrimaryIndex) searcher).getKeys();
			int idx = keys.size();
			for(int idx_finder = 0; idx_finder < keys.size(); idx_finder++){
				if( keys.get(idx_finder) > key){
					idx = idx_finder;
					break;
				}
			}

				System.out.println("<index>");
				for(int k: keys){
					System.out.println(k);
				}
				System.out.println("</index>");
				LibraryNode child = ((LibraryNodePrimaryIndex) searcher).getChildrenAt(idx);
				searcher = child;

		}
		ArrayList<CengBook> books =((LibraryNodeLeaf) searcher).getbooks();
		CengBook holder = null;
		for(CengBook book: books){
			if(book.key() == key){
				holder = book;
				break;
			}
		}

		if(holder == null){
			System.out.println("No match for " + key);
		}
		else{
			System.out.println("<data>");
			System.out.println("<record>" + holder.key() + "|" + holder.year() + "|" + holder.name() + "|" + holder.author() + "</record>");
			System.out.println("</data>");
		}


		// add methods to find the book with the searched key in primary B+ tree
		// return value will not be tested, just print as the specicifications say
		return null;
		}


	public void printPrimaryLibrary() {
			printPrimaryLibraryRecursive(primaryRoot);

		// add methods to print the primary B+ tree in Depth-first order
	}
	private void printPrimaryLibraryRecursive(LibraryNode root){
		if(root.getType() == LibraryNodeType.Leaf){
			System.out.println("<data>");
			for(int k=0 ; k < ((LibraryNodeLeaf)root).bookCount();k++ ){
				CengBook book = ((LibraryNodeLeaf)root).bookAtIndex(k);
				System.out.println("<record>"+book.key()+"|"+book.year()+"|"+book.name()+"|"+book.author()+"</record>");
			}
			System.out.println("</data>");
		}
		else{
			System.out.println("<index>");
			for(int k=0; k<((LibraryNodePrimaryIndex)root).keyCount();k++){
				System.out.println(((LibraryNodePrimaryIndex)root).keyAtIndex(k));
			}
				System.out.println("</index>");
			for(int i=0; i<((LibraryNodePrimaryIndex)root).keyCount()+1;i++){
				printPrimaryLibraryRecursive(((LibraryNodePrimaryIndex)root).getChildrenAt(i));
			}

		}

	}

	public void printSecondaryLibrary() {
		printSecondaryLibraryRecursive(secondaryRoot);
		// add methods to print the secondary B+ tree in Depth-first order

	}
		private void printSecondaryLibraryRecursive(LibraryNode root){
		if(root.getType() == LibraryNodeType.Leaf){
			System.out.println("<data>");
			for(int k=0 ; k < ((LibraryNodeLeaf)root).bookCount();k++ ){
				CengBook book = ((LibraryNodeLeaf)root).bookAtIndex(k);
				System.out.println("<record>"+book.key()+"|"+book.year()+"|"+book.name()+"|"+book.author()+"</record>");
			}
			System.out.println("</data>");
		}
		else{
			System.out.println("<index>");
			for(int k=0; k<((LibraryNodeSecondaryIndex)root).keyCount();k++){
				System.out.println(((LibraryNodeSecondaryIndex)root).yearAtIndex(k)+"|"+((LibraryNodeSecondaryIndex)root).keyAtIndex(k));
			}
				System.out.println("</index>");
			for(int i=0; i<((LibraryNodeSecondaryIndex)root).keyCount()+1;i++){
				printSecondaryLibraryRecursive(((LibraryNodeSecondaryIndex)root).getChildrenAt(i));
			}

		}
	}

	// Extra functions if needed
}
