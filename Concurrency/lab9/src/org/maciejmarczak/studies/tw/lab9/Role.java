package org.maciejmarczak.studies.tw.lab9;

enum Role {
	READER, WRITER_ADD, WRITER_REMOVE;

	@Override
	public String toString() {
		return this.name();
	}
}
