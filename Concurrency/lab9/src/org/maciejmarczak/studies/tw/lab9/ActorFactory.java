package org.maciejmarczak.studies.tw.lab9;

public class ActorFactory {
	public static Actor getActor(NodeList list, Object o, Role role, int n, int it) {
		
		class StringBuilder {
			String getSuccessString() {
				return role + " number " + it + " succeded.";
			}
			String getFailureString() {
				return role + " number " + it + " failed.";
			}
		}
		
		Actor actor;
		StringBuilder sb = new StringBuilder();
		
		switch (role) {
			case READER: actor = new Actor(o, list::contains, n, sb.getSuccessString(), sb.getFailureString()); break;
			case WRITER_ADD: actor = new Actor(o, list::add, n, sb.getSuccessString(), sb.getFailureString()); break;
			case WRITER_REMOVE: actor = new Actor(o, list::remove, n, sb.getSuccessString(), sb.getFailureString()); break;
			default: actor = null;
		}
		
		return actor;		
	};
}
