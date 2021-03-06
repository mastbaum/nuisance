#include "NuisKey.h"
#include "GeneralUtils.h"
#include "NuisConfig.h"

nuiskey::nuiskey(std::string const &name) {
  fNode = Config::Get().CreateNode(name);
}

std::string nuiskey::GetS(std::string const &name) {
  return Config::Get().GetS(fNode, name);
};

void nuiskey::Print() { Config::Get().PrintXML(fNode); }

int nuiskey::GetI(std::string const &name) {
  return Config::Get().GetI(fNode, name);
};

double nuiskey::GetD(std::string const &name) {
  return Config::Get().GetD(fNode, name);
};

bool nuiskey::GetB(std::string const &name) {
  return Config::Get().GetB(fNode, name);
};

void nuiskey::Set(std::string const &name, std::string const &newval) {
  Config::Get().SetS(fNode, name, newval);
}
void nuiskey::Set(std::string const &name, int newval) {
  Config::Get().SetI(fNode, name, newval);
}
void nuiskey::Set(std::string const &name, double newval) {
  Config::Get().SetD(fNode, name, newval);
}
void nuiskey::Set(std::string const &name, bool newval) {
  Config::Get().SetB(fNode, name, newval);
}

void nuiskey::SetS(std::string const &name, std::string const &newval) {
  Config::Get().SetS(fNode, name, newval);
}
void nuiskey::Set(std::string const &name, char const *newval) {
  Config::Get().SetS(fNode, name, newval);
}
void nuiskey::SetI(std::string const &name, int newval) {
  Config::Get().SetI(fNode, name, newval);
}
void nuiskey::SetD(std::string const &name, double newval) {
  Config::Get().SetD(fNode, name, newval);
}
void nuiskey::SetB(std::string const &name, bool newval) {
  Config::Get().SetB(fNode, name, newval);
}

std::vector<std::string> nuiskey::GetAllKeys() {
  return Config::Get().GetAllKeysForNode(fNode);
}

std::vector<nuiskey> nuiskey::GetListOfChildNodes(std::string const &filter) {
  std::vector<XMLNodePointer_t> nodelist =
      Config::Get().GetNodes(fNode, filter);
  std::vector<nuiskey> keylist;
  for (size_t n_it = 0; n_it < nodelist.size(); ++n_it) {
    keylist.push_back(nuiskey(nodelist[n_it]));
  }
  return keylist;
}

std::vector<std::string> nuiskey::GetVS(std::string const &name,
                                        const char *del) {
  return Config::Get().GetVS(fNode, name, del);
};

std::vector<int> nuiskey::GetVI(std::string const &name, const char *del) {
  return Config::Get().GetVI(fNode, name, del);
};

std::vector<double> nuiskey::GetVD(std::string const &name, const char *del) {
  return Config::Get().GetVD(fNode, name, del);
};

std::vector<nuiskey> Config::QueryKeys(std::string const &type,
                                       std::string const &test1) {
  // Get Vector of nodes
  std::vector<XMLNodePointer_t> nodelist = Config::Get().GetNodes(type);

  // Convert List into a key list for easier access
  std::vector<nuiskey> keylist;
  for (std::vector<XMLNodePointer_t>::const_iterator iter = nodelist.begin();
       iter != nodelist.end(); iter++) {
    // Create new key
    nuiskey newkey = nuiskey(*iter);

    // Add test1
    if (!test1.empty()) {
      std::vector<std::string> testvect = GeneralUtils::ParseToStr(test1, "=");
      if (testvect.size() < 2) continue;
      if (newkey.GetS(testvect[0]) != testvect[1]) continue;
    }

    // Save node as nuiskey
    keylist.push_back(newkey);
  }

  // Return list of keys
  return keylist;
}

nuiskey Config::QueryLastKey(std::string const &type,
                             std::string const &test1) {
  // Loop over all for now because I'm lazy...
  std::vector<nuiskey> allkeys = Config::QueryKeys(type, test1);
  if (allkeys.size() < 1)
    return nuiskey();
  else
    return allkeys[allkeys.size() - 1];
}

nuiskey Config::QueryFirstKey(std::string const &type,
                              std::string const &test1) {
  // Loop over all for now because I'm lazy...
  std::vector<nuiskey> allkeys = Config::QueryKeys(type, test1);
  if (allkeys.size() < 1)
    return nuiskey();
  else
    return allkeys[allkeys.size() - 1];
}

bool nuiskey::Has(std::string const &name) {
  return Config::Get().Has(fNode, name);
}

std::string nuiskey::GetElementName() {
  return Config::Get().GetElementName(fNode);
}

nuiskey Config::CreateKey(std::string const &name) {
  return nuiskey(Config::Get().CreateNode(name));
}
