
// XML_reader::Tag_readable::Tag_readable(XML_reader& reader) {

// 	this->encapsulated = reader.Tree_content;

// }

// bool XML_reader::Tag_readable::Exist_Nested_tag(const string& name_nested) {

// 	for (auto it = this->encapsulated->nested_tag.begin(); it != this->encapsulated->nested_tag.end(); it++) {
// 		if ((*it)->name.compare(name_nested) == 0) return true;
// 	}
// 	return false;

// }

// void XML_reader::Tag_readable::Get_Nested(const string& name_nested, list<Tag_readable>* nested) {

// 	nested->clear();
// 	for (list<__Tag*>::iterator it = this->encapsulated->nested_tag.begin(); it != this->encapsulated->nested_tag.end(); it++) {
// 		if ((*it)->name.compare(name_nested) == 0) nested->push_back(Tag_readable(*it));
// 	}

// }

// list<XML_reader::Tag_readable>	XML_reader::Tag_readable::Get_Nested(const string& name_nested) {

// 	list<Tag_readable> temp;
// 	this->Get_Nested(name_nested, &temp);
// 	return temp;

// }

// XML_reader::Tag_readable XML_reader::Tag_readable::Get_Nested_first_found(const string& name_nested) {

// 	XML_reader::Tag_readable res;
// 	for (list<__Tag*>::iterator it = this->encapsulated->nested_tag.begin(); it != this->encapsulated->nested_tag.end(); it++) {
// 		if ((*it)->name.compare(name_nested) == 0) {
// 			res.encapsulated = *it;
// 			return res;
// 		}
// 	}
// #ifdef __USE_THROW
// 	throw 0;
// #else
// 	cout << "not able to find nested tag " << name_nested << " in tag " << this->encapsulated->name << " at line " << this->encapsulated->line_in_file << endl;
// #endif
// 	return Tag_readable();

// }

// XML_reader::Tag_readable XML_reader::Tag_readable::Get_Nested(const list<string>& path) {

// 	__Tag* cursor = this->encapsulated;
// 	for (auto it = path.begin(); it != path.end(); it++) {
// 		Tag_readable attual(cursor);
// 		attual = attual.Get_Nested_first_found(*it);
// 		cursor = attual.encapsulated;
// 	}

// 	return Tag_readable(cursor);

// }

// void XML_reader::Tag_readable::Get_all_nested(std::list<Tag_readable>* nested_tags) {

// 	nested_tags->clear();
// 	for (auto it = this->encapsulated->nested_tag.begin(); it != this->encapsulated->nested_tag.end(); it++)
// 		nested_tags->push_back(Tag_readable(*it));

// }




// std::list<std::string>	XML_reader::Tag_readable::Get_attribute_names() {

// 	list<string> names;
// 	for (auto it = this->encapsulated->fields.begin(); it != this->encapsulated->fields.end(); it++)
// 		names.push_back(it->name);
// 	return names;

// }

// bool XML_reader::Tag_readable::Exist_Field(const string& name_field) {

// 	for (auto it = this->encapsulated->fields.begin(); it != this->encapsulated->fields.end(); it++) {
// 		if (it->name.compare(name_field) == 0) return true;
// 	}
// 	return false;

// }

// const string* XML_reader::Tag_readable::Get_Attribute_first_found(const string& name) {

// 	for (list<__Tag::Field>::iterator it = this->encapsulated->fields.begin(); it != this->encapsulated->fields.end(); it++) {
// 		if (it->name.compare(name) == 0) {
// 			return &it->content;
// 		}
// 	}
// #ifdef __USE_THROW
// 	throw 0;
// #else
// 	cout << "not able to find field " << name << " in tag " << this->encapsulated->name << " at line " << this->encapsulated->line_in_file << endl;
// #endif
// 	return NULL;

// }

// void  XML_reader::Tag_readable::Get_all_Attributes(list<string>* field_names, list<string>* fields) {
	
// 	field_names->clear();
// 	fields->clear();
// 	for (auto it = this->encapsulated->fields.begin(); it != this->encapsulated->fields.end(); it++) {
// 		field_names->push_back(it->name);
// 		fields->push_back(it->content);
// 	}

// }

// list<string>  XML_reader::Tag_readable::Get_Attributes(const string& field_name) {

// 	list<string> vals;
// 	this->Get_Attributes(field_name, &vals);
// 	return vals;

// }

// void  XML_reader::Tag_readable::Get_Attributes(const string& field_name, list<string>* results) {

// 	results->clear();
// 	for (auto it = this->encapsulated->fields.begin(); it != this->encapsulated->fields.end(); it++) {
// 		if (it->name.compare(field_name) == 0) {
// 			results->push_back(it->content);
// 		}
// 	}

// }






// void XML_reader::Tag_readable::Set_attribute_name(const std::string& field_name, const std::string& new_name) {

// 	for (auto it = this->encapsulated->fields.begin(); it != this->encapsulated->fields.end(); it++) {
// 		if (it->name.compare(field_name) == 0)
// 			it->name = new_name;
// 	}

// }

// void XML_reader::Tag_readable::Set_attribute_name(const std::string& name_attribute, const std::string& val_attribute, const std::string& new_name_attribute) {

// 	for (auto it = this->encapsulated->fields.begin(); it != this->encapsulated->fields.end(); it++) {
// 		if ((it->name.compare(name_attribute) == 0) && (it->content.compare(val_attribute) == 0))
// 			it->name = new_name_attribute;
// 	}

// }

// void XML_reader::Tag_readable::Set_attribute_value(const std::string& field_name, const std::list<std::string>& new_vals) {

// 	list<string*> val_to_change;
// 	for (auto it = this->encapsulated->fields.begin(); it != this->encapsulated->fields.end(); it++) {
// 		if (it->name.compare(field_name) == 0) 
// 			val_to_change.push_back(&it->content);
// 	}

// 	if (val_to_change.size() != new_vals.size()) {
// 		system("echo wrong number of values, ignored");
// 		return;
// 	}

// 	auto it = new_vals.begin();
// 	for (auto it_v = val_to_change.begin(); it_v != val_to_change.end(); it_v++) {
// 		**it_v = *it;
// 		it++;
// 	}

// }

// void XML_reader::Tag_readable::Set_attribute_value(const std::string& name_attribute, const std::string& val_attribute, const std::string& new_value) {

// 	for (auto it = this->encapsulated->fields.begin(); it != this->encapsulated->fields.end(); it++) {
// 		if ((it->name.compare(name_attribute) == 0) && (it->content.compare(val_attribute) == 0))
// 			it->content = new_value;
// 	}

// }

// void XML_reader::Tag_readable::Remove_Attribute(const std::string& field_name, const std::string& value) {

// 	for (auto it = this->encapsulated->fields.begin(); it != this->encapsulated->fields.end(); it++) {
// 		if ((it->name.compare(field_name) == 0) && (it->content.compare(value) == 0)) {
// 			it = this->encapsulated->fields.erase(it);
// 			return;
// 		}
// 	}

// }

// void XML_reader::Tag_readable::Remove_Attribute(const std::string& field_name) {

// 	auto it = this->encapsulated->fields.begin();
// 	while (it != this->encapsulated->fields.end()) {
// 		if (it->name.compare(field_name) == 0)
// 			it = this->encapsulated->fields.erase(it);
// 		else
// 			it++;
// 	}

// }

// void XML_reader::Tag_readable::Add_Attribute(const std::string& field_name, const std::string& content) {

// 	this->encapsulated->fields.push_back(__Tag::Field());
// 	this->encapsulated->fields.back().name = field_name;
// 	this->encapsulated->fields.back().content = content;

// }

// XML_reader::Tag_readable XML_reader::Tag_readable::Add_Nested_and_return_created(const std::string& tag_name) {

// 	__Tag* created = new __Tag(tag_name);
// 	this->encapsulated->nested_tag.push_back(created);
// 	return Tag_readable(created);

// }

// void XML_reader::Tag_readable::Add_Nested(const std::string& tag_name) {

// 	this->Add_Nested_and_return_created(tag_name);

// }

// void XML_reader::Tag_readable::Remove() {

// 	if (this->encapsulated->father == NULL) {
// 		system("echo You cannot remove the root, ignored");
// 		return;
// 	}

// 	for (auto it = this->encapsulated->father->nested_tag.begin();
// 		it != this->encapsulated->father->nested_tag.end(); it++) {
// 		if (*it == this->encapsulated) {
// 			it = this->encapsulated->father->nested_tag.erase(it);
// 			break;
// 		}
// 	}

// 	delete this->encapsulated;

// 	this->encapsulated = NULL;

// }