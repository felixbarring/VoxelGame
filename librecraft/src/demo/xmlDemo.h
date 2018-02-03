
#ifndef SOURCE_DIRECTORY__SRC_DEMO_XMLDEMO_H_
#define SOURCE_DIRECTORY__SRC_DEMO_XMLDEMO_H_

#include <string>
#include <iostream>

#include "iDemo.h"
#include "../dependencies/tinyXml2/tinyxml2.h"

using std::string;

namespace demo {

class XmlDemo : public IDemo {
public:

  void runDemo() override {
    using namespace std;
    using namespace tinyxml2;

    string elementCool("CoolValue");
    string elementCoolValue("I'm pretty good.");

    string elementList("List");
    string elementListAttribute1("items");
    string elementListAttribute2("isCakeTasy");
    string elementListAttribute1Value("5");
    string elementListAttribute2Value("yes");

    string elementItem("Item");

    string fileName("xmlDemo.xml");

    {
      XMLDocument doc;
      XMLNode* root{doc.NewElement("xml")};
      doc.InsertFirstChild(root);

      XMLElement* element{doc.NewElement(elementCool.c_str())};
      element->SetText(elementCoolValue.c_str());
      XMLNode* value{element};
      root->InsertEndChild(value);

      XMLElement* listElement{doc.NewElement(elementList.c_str())};
      listElement->SetAttribute(elementListAttribute1.c_str(),
          elementListAttribute1Value.c_str());
      listElement->SetAttribute(elementListAttribute2.c_str(),
          elementListAttribute2Value.c_str());

      for (int i{0}; i < 5; ++i) {
        XMLElement* pListElement{doc.NewElement(elementItem.c_str())};
        pListElement->SetText(i);

        listElement->InsertEndChild(pListElement);
      }
      root->InsertEndChild(listElement);

      doc.SaveFile(fileName.c_str());
    }

    // Time to load the file :-)

    {
      XMLDocument doc;
      XMLError error = doc.LoadFile(fileName.c_str());

      cout << "errorCode is: " + to_string(error) << endl;

      XMLNode* root{doc.FirstChild()};

      if (!root) {
        cout << "No root node." << endl;
        return;
      }

      XMLElement* element{root->FirstChildElement(elementCool.c_str())};
      string value{element->GetText()};

      cout << value << endl;

      XMLElement* listElement{root->FirstChildElement(elementList.c_str())};
      string attribute1{listElement->Attribute(elementListAttribute1.c_str())};
      string attribute2{listElement->Attribute(elementListAttribute2.c_str())};

      cout << "Attribute 1 for list is: " + attribute1 << endl;
      cout << "Attribute 2 for list is: " + attribute2 << endl;

      XMLElement* listItemElement{listElement->FirstChildElement(elementItem.c_str())};

      vector<int> items{};

      while (listItemElement != nullptr) {
        int value;
        listItemElement->QueryIntText(&value);
        items.push_back(value);
        cout << value << endl;
        listItemElement = listItemElement->NextSiblingElement(elementItem.c_str());
      }
    }

  }

};

}

#endif /* SOURCE_DIRECTORY__SRC_DEMO_XMLDEMO_H_ */
