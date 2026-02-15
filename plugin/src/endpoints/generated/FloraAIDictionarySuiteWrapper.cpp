#include "FloraAIDictionarySuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIDictionarySuite* sDictionary;

namespace Flora {
namespace AIDictionarySuite {

nlohmann::json CreateDictionary(const nlohmann::json& params) {
    nlohmann::json response;

    // Output handle: dictionary
    AIDictionaryRef dictionary = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->CreateDictionary(&dictionary);
    if (err != kNoErr) {
        throw std::runtime_error("CreateDictionary failed with error: " + std::to_string(err));
    }

    // Marshal output handle: dictionary
    if (dictionary) {
        response["dictionary"] = HandleManager::dictionaries.Register(dictionary);
    } else {
        response["dictionary"] = -1;
    }

    return response;
}

nlohmann::json CreateDictionaryFromJSONFile(const nlohmann::json& params) {
    nlohmann::json response;

    // Output handle: dictionary
    AIDictionaryRef dictionary = nullptr;
    // Input file path: file
    ai::FilePath file(ai::UnicodeString(params["file"].get<std::string>()));

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->CreateDictionaryFromJSONFile(&dictionary, file);
    if (err != kNoErr) {
        throw std::runtime_error("CreateDictionaryFromJSONFile failed with error: " + std::to_string(err));
    }

    // Marshal output handle: dictionary
    if (dictionary) {
        response["dictionary"] = HandleManager::dictionaries.Register(dictionary);
    } else {
        response["dictionary"] = -1;
    }

    return response;
}

nlohmann::json Clone(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: src
    ConstAIDictionaryRef src_val = HandleManager::dictionaries.Get(params["src"].get<int32_t>());
    if (!src_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'src'");
    }
    // Output handle: dst
    AIDictionaryRef dst = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->Clone(src_val, &dst);
    if (err != kNoErr) {
        throw std::runtime_error("Clone failed with error: " + std::to_string(err));
    }

    // Marshal output handle: dst
    if (dst) {
        response["dst"] = HandleManager::dictionaries.Register(dst);
    } else {
        response["dst"] = -1;
    }

    return response;
}

nlohmann::json Copy(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dict
    AIDictionaryRef dict_val = HandleManager::dictionaries.Get(params["dict"].get<int32_t>());
    if (!dict_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dict'");
    }
    // Input handle: src
    ConstAIDictionaryRef src_val = HandleManager::dictionaries.Get(params["src"].get<int32_t>());
    if (!src_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'src'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->Copy(dict_val, src_val);
    if (err != kNoErr) {
        throw std::runtime_error("Copy failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Begin(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dict
    ConstAIDictionaryRef dict_val = HandleManager::dictionaries.Get(params["dict"].get<int32_t>());
    if (!dict_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dict'");
    }
    // Output handle: iterator
    AIDictionaryIterator iterator = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->Begin(dict_val, &iterator);
    if (err != kNoErr) {
        throw std::runtime_error("Begin failed with error: " + std::to_string(err));
    }

    // Marshal output handle: iterator
    if (iterator) {
        response["iterator"] = HandleManager::dictIterators.Register(iterator);
    } else {
        response["iterator"] = -1;
    }

    return response;
}

nlohmann::json DeleteEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->DeleteEntry(dictionary_val, key_val);
    if (err != kNoErr) {
        throw std::runtime_error("DeleteEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetEntryType(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    ConstAIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Output primitive: entryType
    AIEntryType entryType{};

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->GetEntryType(dictionary_val, key_val, &entryType);
    if (err != kNoErr) {
        throw std::runtime_error("GetEntryType failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: entryType
    response["entryType"] = entryType;

    return response;
}

nlohmann::json CopyEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary1
    ConstAIDictionaryRef dictionary1_val = HandleManager::dictionaries.Get(params["dictionary1"].get<int32_t>());
    if (!dictionary1_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary1'");
    }
    // Input handle: dictionary2
    AIDictionaryRef dictionary2_val = HandleManager::dictionaries.Get(params["dictionary2"].get<int32_t>());
    if (!dictionary2_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary2'");
    }
    // Input handle: key1
    AIDictKey key1_val = HandleManager::dictKeys.Get(params["key1"].get<int32_t>());
    if (!key1_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key1'");
    }
    // Input handle: key2
    AIDictKey key2_val = HandleManager::dictKeys.Get(params["key2"].get<int32_t>());
    if (!key2_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key2'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->CopyEntry(dictionary1_val, dictionary2_val, key1_val, key2_val);
    if (err != kNoErr) {
        throw std::runtime_error("CopyEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json MoveEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary1
    AIDictionaryRef dictionary1_val = HandleManager::dictionaries.Get(params["dictionary1"].get<int32_t>());
    if (!dictionary1_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary1'");
    }
    // Input handle: dictionary2
    AIDictionaryRef dictionary2_val = HandleManager::dictionaries.Get(params["dictionary2"].get<int32_t>());
    if (!dictionary2_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary2'");
    }
    // Input handle: key1
    AIDictKey key1_val = HandleManager::dictKeys.Get(params["key1"].get<int32_t>());
    if (!key1_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key1'");
    }
    // Input handle: key2
    AIDictKey key2_val = HandleManager::dictKeys.Get(params["key2"].get<int32_t>());
    if (!key2_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key2'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->MoveEntry(dictionary1_val, dictionary2_val, key1_val, key2_val);
    if (err != kNoErr) {
        throw std::runtime_error("MoveEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SwapEntries(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary1
    AIDictionaryRef dictionary1_val = HandleManager::dictionaries.Get(params["dictionary1"].get<int32_t>());
    if (!dictionary1_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary1'");
    }
    // Input handle: dictionary2
    AIDictionaryRef dictionary2_val = HandleManager::dictionaries.Get(params["dictionary2"].get<int32_t>());
    if (!dictionary2_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary2'");
    }
    // Input handle: key1
    AIDictKey key1_val = HandleManager::dictKeys.Get(params["key1"].get<int32_t>());
    if (!key1_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key1'");
    }
    // Input handle: key2
    AIDictKey key2_val = HandleManager::dictKeys.Get(params["key2"].get<int32_t>());
    if (!key2_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key2'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->SwapEntries(dictionary1_val, dictionary2_val, key1_val, key2_val);
    if (err != kNoErr) {
        throw std::runtime_error("SwapEntries failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetArtEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    ConstAIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Output handle: art
    AIArtHandle art = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->GetArtEntry(dictionary_val, key_val, &art);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtEntry failed with error: " + std::to_string(err));
    }

    // Marshal output handle: art
    if (art) {
        response["art"] = HandleManager::art.Register(art);
    } else {
        response["art"] = -1;
    }

    return response;
}

nlohmann::json NewArtEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input primitive: type
    ai::int16 type = params["type"].get<int16_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->NewArtEntry(dictionary_val, key_val, type);
    if (err != kNoErr) {
        throw std::runtime_error("NewArtEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json MoveArtToEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->MoveArtToEntry(dictionary_val, key_val, art_val);
    if (err != kNoErr) {
        throw std::runtime_error("MoveArtToEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json MoveEntryToArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input primitive: paintOrder
    ai::int16 paintOrder = params["paintOrder"].get<int16_t>();
    // Input handle (optional): prep
    AIArtHandle prep_val = nullptr;
    if (params.contains("prep") && !params["prep"].is_null()) {
        int32_t prep_id = params["prep"].get<int32_t>();
        if (prep_id >= 0) {
            prep_val = HandleManager::art.Get(prep_id);
        }
    }
    // Output handle: art
    AIArtHandle art = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->MoveEntryToArt(dictionary_val, key_val, paintOrder, prep_val, &art);
    if (err != kNoErr) {
        throw std::runtime_error("MoveEntryToArt failed with error: " + std::to_string(err));
    }

    // Marshal output handle: art
    if (art) {
        response["art"] = HandleManager::art.Register(art);
    } else {
        response["art"] = -1;
    }

    return response;
}

nlohmann::json CopyArtToEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->CopyArtToEntry(dictionary_val, key_val, art_val);
    if (err != kNoErr) {
        throw std::runtime_error("CopyArtToEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json CopyEntryToArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    ConstAIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input primitive: paintOrder
    ai::int16 paintOrder = params["paintOrder"].get<int16_t>();
    // Input handle (optional): prep
    AIArtHandle prep_val = nullptr;
    if (params.contains("prep") && !params["prep"].is_null()) {
        int32_t prep_id = params["prep"].get<int32_t>();
        if (prep_id >= 0) {
            prep_val = HandleManager::art.Get(prep_id);
        }
    }
    // Output handle: art
    AIArtHandle art = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->CopyEntryToArt(dictionary_val, key_val, paintOrder, prep_val, &art);
    if (err != kNoErr) {
        throw std::runtime_error("CopyEntryToArt failed with error: " + std::to_string(err));
    }

    // Marshal output handle: art
    if (art) {
        response["art"] = HandleManager::art.Register(art);
    } else {
        response["art"] = -1;
    }

    return response;
}

nlohmann::json SetEntryToLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->SetEntryToLayer(dictionary_val, key_val, layer_val);
    if (err != kNoErr) {
        throw std::runtime_error("SetEntryToLayer failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetLayerToEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    ConstAIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input primitive: paintOrder
    ai::int16 paintOrder = params["paintOrder"].get<int16_t>();
    // Input handle (optional): prep
    AILayerHandle prep_val = nullptr;
    if (params.contains("prep") && !params["prep"].is_null()) {
        int32_t prep_id = params["prep"].get<int32_t>();
        if (prep_id >= 0) {
            prep_val = HandleManager::layers.Get(prep_id);
        }
    }
    // Output handle: layer
    AILayerHandle layer = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->SetLayerToEntry(dictionary_val, key_val, paintOrder, prep_val, &layer);
    if (err != kNoErr) {
        throw std::runtime_error("SetLayerToEntry failed with error: " + std::to_string(err));
    }

    // Marshal output handle: layer
    if (layer) {
        response["layer"] = HandleManager::layers.Register(layer);
    } else {
        response["layer"] = -1;
    }

    return response;
}

nlohmann::json Set(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->Set(dictionary_val, key_val, entry_val);
    if (err != kNoErr) {
        throw std::runtime_error("Set failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetBooleanEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    ConstAIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Output primitive: value
    AIBoolean value{};

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->GetBooleanEntry(dictionary_val, key_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("GetBooleanEntry failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: value
    response["value"] = static_cast<bool>(value);

    return response;
}

nlohmann::json SetBooleanEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input primitive: value
    AIBoolean value = params["value"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->SetBooleanEntry(dictionary_val, key_val, value);
    if (err != kNoErr) {
        throw std::runtime_error("SetBooleanEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetIntegerEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    ConstAIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Output primitive: value
    ai::int32 value{};

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->GetIntegerEntry(dictionary_val, key_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("GetIntegerEntry failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: value
    response["value"] = value;

    return response;
}

nlohmann::json SetIntegerEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input primitive: value
    ai::int32 value = params["value"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->SetIntegerEntry(dictionary_val, key_val, value);
    if (err != kNoErr) {
        throw std::runtime_error("SetIntegerEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetPointerEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    ConstAIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Output unknown: value
    ai::intptr value{};

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->GetPointerEntry(dictionary_val, key_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("GetPointerEntry failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: value

    return response;
}

nlohmann::json SetPointerEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Unknown type: value (ai::intptr)
    // WARNING: Using default initialization
    ai::intptr value{};

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->SetPointerEntry(dictionary_val, key_val, value);
    if (err != kNoErr) {
        throw std::runtime_error("SetPointerEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetRealEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    ConstAIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Output primitive: value
    AIReal value{};

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->GetRealEntry(dictionary_val, key_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("GetRealEntry failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: value
    response["value"] = value;

    return response;
}

nlohmann::json SetRealEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input primitive: value
    AIReal value = params["value"].get<double>();

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->SetRealEntry(dictionary_val, key_val, value);
    if (err != kNoErr) {
        throw std::runtime_error("SetRealEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetStringEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input string: value
    std::string value_str = params["value"].get<std::string>();
    const char* value = value_str.c_str();

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->SetStringEntry(dictionary_val, key_val, value);
    if (err != kNoErr) {
        throw std::runtime_error("SetStringEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDictEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    ConstAIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Output handle: value
    AIDictionaryRef value = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->GetDictEntry(dictionary_val, key_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("GetDictEntry failed with error: " + std::to_string(err));
    }

    // Marshal output handle: value
    if (value) {
        response["value"] = HandleManager::dictionaries.Register(value);
    } else {
        response["value"] = -1;
    }

    return response;
}

nlohmann::json SetDictEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input handle: value
    AIDictionaryRef value_val = HandleManager::dictionaries.Get(params["value"].get<int32_t>());
    if (!value_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'value'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->SetDictEntry(dictionary_val, key_val, value_val);
    if (err != kNoErr) {
        throw std::runtime_error("SetDictEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetArrayEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    ConstAIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Output handle: value
    AIArrayRef value = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->GetArrayEntry(dictionary_val, key_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("GetArrayEntry failed with error: " + std::to_string(err));
    }

    // Marshal output handle: value
    if (value) {
        response["value"] = HandleManager::arrays.Register(value);
    } else {
        response["value"] = -1;
    }

    return response;
}

nlohmann::json SetArrayEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input handle: value
    AIArrayRef value_val = HandleManager::arrays.Get(params["value"].get<int32_t>());
    if (!value_val) {
        throw std::runtime_error("Invalid AIArrayRef handle for parameter 'value'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->SetArrayEntry(dictionary_val, key_val, value_val);
    if (err != kNoErr) {
        throw std::runtime_error("SetArrayEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetUnicodeStringEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    ConstAIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Output string: value
    ai::UnicodeString value;

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->GetUnicodeStringEntry(dictionary_val, key_val, value);
    if (err != kNoErr) {
        throw std::runtime_error("GetUnicodeStringEntry failed with error: " + std::to_string(err));
    }

    // Marshal output string: value
    response["value"] = value.as_UTF8();

    return response;
}

nlohmann::json SetUnicodeStringEntry(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    AIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'dictionary'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Input string: value
    ai::UnicodeString value(params["value"].get<std::string>());

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->SetUnicodeStringEntry(dictionary_val, key_val, value);
    if (err != kNoErr) {
        throw std::runtime_error("SetUnicodeStringEntry failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json TouchArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dictionary
    ConstAIDictionaryRef dictionary_val = HandleManager::dictionaries.Get(params["dictionary"].get<int32_t>());
    if (!dictionary_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dictionary'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->TouchArt(dictionary_val);
    if (err != kNoErr) {
        throw std::runtime_error("TouchArt failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Find(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dict
    ConstAIDictionaryRef dict_val = HandleManager::dictionaries.Get(params["dict"].get<int32_t>());
    if (!dict_val) {
        throw std::runtime_error("Invalid ConstAIDictionaryRef handle for parameter 'dict'");
    }
    // Input handle: key
    AIDictKey key_val = HandleManager::dictKeys.Get(params["key"].get<int32_t>());
    if (!key_val) {
        throw std::runtime_error("Invalid AIDictKey handle for parameter 'key'");
    }
    // Output handle: iterator
    AIDictionaryIterator iterator = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sDictionary->Find(dict_val, key_val, &iterator);
    if (err != kNoErr) {
        throw std::runtime_error("Find failed with error: " + std::to_string(err));
    }

    // Marshal output handle: iterator
    if (iterator) {
        response["iterator"] = HandleManager::dictIterators.Register(iterator);
    } else {
        response["iterator"] = -1;
    }

    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "CreateDictionary") {
        return CreateDictionary(params);
    } else if (method == "CreateDictionaryFromJSONFile") {
        return CreateDictionaryFromJSONFile(params);
    } else if (method == "Clone") {
        return Clone(params);
    } else if (method == "Copy") {
        return Copy(params);
    } else if (method == "Begin") {
        return Begin(params);
    } else if (method == "DeleteEntry") {
        return DeleteEntry(params);
    } else if (method == "GetEntryType") {
        return GetEntryType(params);
    } else if (method == "CopyEntry") {
        return CopyEntry(params);
    } else if (method == "MoveEntry") {
        return MoveEntry(params);
    } else if (method == "SwapEntries") {
        return SwapEntries(params);
    } else if (method == "GetArtEntry") {
        return GetArtEntry(params);
    } else if (method == "NewArtEntry") {
        return NewArtEntry(params);
    } else if (method == "MoveArtToEntry") {
        return MoveArtToEntry(params);
    } else if (method == "MoveEntryToArt") {
        return MoveEntryToArt(params);
    } else if (method == "CopyArtToEntry") {
        return CopyArtToEntry(params);
    } else if (method == "CopyEntryToArt") {
        return CopyEntryToArt(params);
    } else if (method == "SetEntryToLayer") {
        return SetEntryToLayer(params);
    } else if (method == "SetLayerToEntry") {
        return SetLayerToEntry(params);
    } else if (method == "Set") {
        return Set(params);
    } else if (method == "GetBooleanEntry") {
        return GetBooleanEntry(params);
    } else if (method == "SetBooleanEntry") {
        return SetBooleanEntry(params);
    } else if (method == "GetIntegerEntry") {
        return GetIntegerEntry(params);
    } else if (method == "SetIntegerEntry") {
        return SetIntegerEntry(params);
    } else if (method == "GetPointerEntry") {
        return GetPointerEntry(params);
    } else if (method == "SetPointerEntry") {
        return SetPointerEntry(params);
    } else if (method == "GetRealEntry") {
        return GetRealEntry(params);
    } else if (method == "SetRealEntry") {
        return SetRealEntry(params);
    } else if (method == "SetStringEntry") {
        return SetStringEntry(params);
    } else if (method == "GetDictEntry") {
        return GetDictEntry(params);
    } else if (method == "SetDictEntry") {
        return SetDictEntry(params);
    } else if (method == "GetArrayEntry") {
        return GetArrayEntry(params);
    } else if (method == "SetArrayEntry") {
        return SetArrayEntry(params);
    } else if (method == "GetUnicodeStringEntry") {
        return GetUnicodeStringEntry(params);
    } else if (method == "SetUnicodeStringEntry") {
        return SetUnicodeStringEntry(params);
    } else if (method == "TouchArt") {
        return TouchArt(params);
    } else if (method == "Find") {
        return Find(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIDictionarySuite");
}

} // namespace AIDictionarySuite
} // namespace Flora
