package org.maciejmarczak.studies.tw.lab9;

public class GrainedList extends NodeList {

	private Node guard;
	
	public GrainedList() {
		guard = new Node();
		guard.next = head;
		guard.o = new Object();
	}
	
	@Override
	public boolean contains(Object o) {		
        Node current = guard;
        boolean result = false;

        current.lock.lock();

        while ((current.next != null) && !result) {

            current.next.lock.lock();

            if (current.o == o) {
                result = true;
            }

            current.lock.unlock();
            current = current.next;

        }

        current.lock.unlock();

        return result;
	}

	@Override
	public boolean remove(Object o) {
		Node prev = guard;

        prev.lock.lock();

        Node current = prev.next;
        boolean result = false;

        while (current != null) {

            current.lock.lock();

            if (current.o == o) {
                result = true;
                break;
            }

            prev.lock.unlock();
            prev = current;
            current = current.next;
        }


        if(result == true){
            prev.next = current.next;
            current.lock.unlock();
        }

        prev.lock.unlock();

        return result;
	}

	@Override
	public boolean add(Object o) {
        Node current = guard;

        current.lock.lock();

        while (current.next != null) {
            current.next.lock.lock();
            current.lock.unlock();
            current = current.next;
        }

        Node newNode = new Node();
        newNode.o = o;
        current.next = newNode;

        current.lock.unlock();

        return true;
	}

}
