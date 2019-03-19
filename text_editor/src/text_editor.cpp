#include <string>
#include "test_runner.h"
#include <algorithm>
#include <list>
using namespace std;

class Editor {
 public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor() {}
  void Left();
  void Right();
  void Insert(char token);
  void Cut(size_t tokens = 1);
  void Copy(size_t tokens = 1);
  void Paste();
  string GetText() const;

 private:
  using Iterator = list<char>::iterator;
  list<char> text;
  list<char> buffer;
  int buf_size = 0;
  int num = 0;
  list<char>::iterator pointer = text.begin();;
  size_t size_ = 0;

  Iterator ret_It(Iterator it, int steps) const {
  	while (steps > 0 && it != text.end()) {
  		++it;
  		--steps;
  	}
  	while (steps < 0 && it != text.begin()) {
  		--it;
  		++steps;
  	}
  	return it;
  }
};


void Editor::Right() {
	pointer = ret_It(pointer, 1);
}

void Editor::Left() {
	pointer = ret_It(pointer, -1);
}

void Editor::Insert(char token) {
	text.insert(pointer, token);
}

void Editor::Cut(size_t tokens) {
	auto pos = ret_It(pointer, tokens);
	buffer.assign(pointer, pos);
	pointer = text.erase(pointer, pos);
}

void Editor::Copy(size_t tokens) {
	auto pos = ret_It(pointer, tokens);
	buffer.assign(pointer, pos);
}

void Editor::Paste() {
	text.insert(pointer, buffer.begin(), buffer.end());
}

string Editor::GetText() const {
	return {text.begin(), text.end()};
}
void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);

    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;

    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");

  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}

/*
int main() {
  Editor editor;
  const string text = "hello, world";
  for (char c : text) {
    editor.Insert(c);
  }
  // Текущее состояние редактора: `hello, world|`
  for (size_t i = 0; i < text.size(); ++i) {
    editor.Left();
  }
  // Текущее состояние редактора: `|hello, world`
  editor.Cut(7);
  // Текущее состояние редактора: `|world`
  // в буфере обмена находится текст `hello, `
  for (size_t i = 0; i < 5; ++i) {
    editor.Right();
  }
  // Текущее состояние редактора: `world|`
  editor.Insert(',');
  editor.Insert(' ');
  // Текущее состояние редактора: `world, |`
  editor.Paste();
  // Текущее состояние редактора: `world, hello, |`
  editor.Left();
  editor.Left();
  //Текущее состояние редактора: `world, hello|, `
  editor.Cut(3); // Будут вырезаны 2 символа
  // Текущее состояние редактора: `world, hello|`
  cout << editor.GetText();
}
*/
