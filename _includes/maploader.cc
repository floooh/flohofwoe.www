//------------------------------------------------------------------------------
//  maploader.cc
//  (C) 2012 Bigpoint GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "maploader.h"
#include "io/ioserver.h"

namespace Map
{
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
 */
MapLoader::MapLoader()
{
    
}

//------------------------------------------------------------------------------
/**
 */
MapLoader::~MapLoader()
{
    
}


//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::LoadMap(const URI& fromUri, Ptr<MapData>& outMapData)
{
    bool result = this->Load(fromUri, false, outMapData);
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool 
MapLoader::LoadAdditionalEvents(const URI& fromUri, Array<MapData::Instance>& outInstances, Array<MapData::Group>& outGroups)
{
    Ptr<MapData> tmpMapData = MapData::Create();
    bool result = this->Load(fromUri, true, tmpMapData);
    if (result)
    {
        outInstances = tmpMapData->instances;
        outGroups = tmpMapData->groups;
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::Load(const URI& fromUri, bool loadEvents, Ptr<MapData>& outMapData)
{
    bool result=false;
    Ptr<Stream> stream = IoServer::Instance()->LoadStreamSync(fromUri);
    this->reader = BinaryReader::Create();
    this->reader->SetStream(stream);
    this->reader->SetMemoryMappingEnabled(true);
    if (this->reader->Open())
    {
        this->outMap = outMapData;
        if (loadEvents)
        {
            if (this->TryLoadEvent())
            {
                result=true;
            }
        }
        else
        {
            if (this->TryLoadMap())
            {
                result=true;
            }
        }
        reader->Close();
        this->outMap = 0;
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::TryLoadMap()
{
    bool result = false;
    if (this->ParseHeader() &&
        this->LoadMapInfo() &&
        this->LoadStringTable() &&
        this->LoadSets() &&
        this->LoadEventList() &&
        this->LoadEventMapping() &&
        this->LoadTemplates() &&
        this->LoadInstances() &&
        this->LoadGroups() &&
        this->LoadNavBlockers())
    {
        result = true;
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::TryLoadEvent()
{
    bool result = false;
    if (this->ParseHeader() &&
        this->LoadInstances() &&
        this->LoadGroups())
    {
        result = true;
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::CheckTag(FourCC tag, const char* errorMsg)
{
    FourCC readTag = reader->ReadUInt();
    if (tag != readTag)
    {
        this->error = errorMsg;
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::ParseHeader()
{
    bool result=false;
    if (this->CheckTag('DSOM', "Magic number mismatch ('DSOM' expected)!"))
    {
        uint version = reader->ReadUInt();
        if (version == Version)
        {
            result = true;
        }
        else
        {
            this->error = "Map file format version number mismatch!";
        }
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::LoadMapInfo()
{
    bool result=false;
    if (this->CheckTag('MAPI', "Invalid tag at start of MapInfo block ('MAPI' expected!)"))
    {
        this->outMap->info.gridSize = this->reader->ReadFloat4();
        this->outMap->info.center   = this->reader->ReadFloat4();
        this->outMap->info.extents  = this->reader->ReadFloat4();
        this->outMap->info.mapSizeX = this->reader->ReadShort();
        this->outMap->info.mapSizeY = this->reader->ReadShort();
        this->outMap->info.mapSizeZ = this->reader->ReadShort();
        result = true;
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::LoadStringTable()
{
    bool result=false;
    n_assert(this->outMap->stringTable.IsEmpty());
    if (this->CheckTag('STRT', "Invalid tag at start of StringTable block ('STRT' expected!)"))
    {
        SizeT numStrings = this->reader->ReadUInt();
        SizeT tableSize  = this->reader->ReadUInt();
        if (numStrings > 0)
        {
            this->outMap->stringTable.Reserve(numStrings);
            char* buffer = (char*) Memory::Alloc(Memory::ScratchHeap, tableSize);
            this->reader->ReadRawData(buffer, tableSize);
            const char* ptr = buffer;
            IndexT i;
            for (i = 0; i < numStrings; ++i)
            {
                n_assert(ptr < buffer + tableSize);
                StringAtom strAtom(ptr);
                this->outMap->stringTable.Append(strAtom);
                ptr += String::StrLen(ptr) + 1;
            }
            Memory::Free(Memory::ScratchHeap, buffer);
        }    
        result = true;
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::LoadEventList()
{
    bool result=false;
    if (this->CheckTag('EVET', "Invalid tag at start of EventList block ('EVET' expected!)"))
    {
        uint numEvents = this->reader->ReadUInt();
        this->outMap->eventList.Reserve(numEvents);        
        uint i;
        for (i = 0; i < numEvents; ++i)
        {
            this->outMap->eventList.Append(this->reader->ReadUInt());
        }
        result = true;
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::LoadEventMapping()
{
    bool result=false;
    if (this->CheckTag('EMAL', "Invalid tag at start of EventMapping block ('EMAL' expected!)"))
    {
        uint numEvents = this->reader->ReadUInt();
        this->outMap->eventMapping.Reserve(numEvents);
        uint i;
        for (i = 0; i < numEvents; ++i)
        {
            String mapping = this->reader->ReadString();
            this->outMap->eventMapping.Append(mapping);            
        }
        result = true;
    }    
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::LoadTemplates()
{
    bool result=false;
    if (this->CheckTag('TMPL', "Invalid tag at start of TemplateTable block ('TMPL' expected!)"))
    {
        uint templateCount = this->reader->ReadUInt();        
        if (templateCount > 0)
        {            
            this->outMap->templates.Reserve(templateCount);
            MapData::Template tmpl;
            uint i;
            for (i = 0; i < templateCount; ++i)
            {
                tmpl.gfxResId = this->reader->ReadUShort();
                tmpl.gfxRootNode = this->reader->ReadUShort(); 
                tmpl.phxResId = this->reader->ReadUShort();  
                tmpl.collResId = this->reader->ReadUShort();
                tmpl.sfxEventId = this->reader->ReadUShort();
                tmpl.center  = this->reader->ReadFloat4();
                tmpl.extents = this->reader->ReadFloat4();
                tmpl.collMeshGroupIndex = this->reader->ReadUShort();
                tmpl.type = this->reader->ReadUShort();
                this->outMap->templates.Append(tmpl);
            }
        }
        result = true;
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::LoadInstances()
{
    bool result=false;
    if (this->CheckTag('INST', "Invalid tag at start of InstanceTable block ('INST' expected!)"))
    {
        uint numInsts = this->reader->ReadUInt();        
        if (numInsts > 0)
        {    
            this->outMap->instances.Reserve(numInsts);
            MapData::Instance instance;
            uint i;
            for (i = 0; i < numInsts; ++i)
            {
                instance.pos = this->reader->ReadFloat4();
                instance.rot = this->reader->ReadFloat4();
                instance.useScaling = this->reader->ReadBool();
                if (instance.useScaling)
                {
                    instance.scale = this->reader->ReadFloat4();
                }
                else
                {
                    instance.scale = Math::float4(1,1,1,0);
                }
                instance.useCollide = this->reader->ReadBool();
                instance.isVisibleForNavMeshGen = this->reader->ReadBool();
                instance.templIndex = this->reader->ReadShort();    
                instance.groupIndex = this->reader->ReadShort();    
                instance.dgNameIndex = this->reader->ReadUShort();
                instance.indexToMapping = this->reader->ReadUShort();
                this->outMap->instances.Append(instance);
            }
        }
        result = true;
    }    
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
MapLoader::LoadGroups()
{
    bool result=false;
    if (this->CheckTag('GROP', "Invalid tag at start of GroupTable block ('GROP' expected!)"))
    {
        ushort numInstances;
        ushort numGroups;
        uint groupLen = this->reader->ReadUInt();        
        if (groupLen > 0)
        {               
            MapData::Group grp;

            this->outMap->groups.Reserve(groupLen);
            uint i;
            for (i= 0; i < groupLen; ++i)
            {
                grp.nameId=this->reader->ReadUShort();
                grp.type = this->reader->ReadUShort();
                grp.parentGroupIndex = this->reader->ReadShort();
                numInstances = this->reader->ReadUShort();
                numGroups = this->reader->ReadUShort();                
                
                this->outMap->groups.Append(grp);
                if (numInstances > 0)
                {
                    this->outMap->groups[i].instanceIndices.Reserve(numInstances);
                    uint j;
                    for (j = 0; j < numInstances; ++j)
                    {
                        this->outMap->groups[i].instanceIndices.Append(this->reader->ReadUShort());
                    }
                }

                if (numGroups > 0)
                {
                    this->outMap->groups[i].groupIndices.Reserve(numGroups);
                    uint j;
                    for (j = 0; j < numGroups; ++j)
                    {
                        this->outMap->groups[i].groupIndices.Append(this->reader->ReadUShort());
                    }
                }
            }
        }
        result = true;
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
Map::MapLoader::LoadSets()
{
    if (this->CheckTag('SETT', "Invalid tag at start of SetTable block ('SETT' expected!)"))
    {
        SizeT numSets = reader->ReadUInt();
        IndexT i;
        for (i = 0; i < numSets; i++)
        {
            __attribute__((unused)) ushort setNameIndex = reader->ReadUShort();
            __attribute__((unused)) ushort setType = reader->ReadUShort();
        }
        return true;        
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Map::MapLoader::LoadNavBlockers()
{
    if (this->CheckTag('NAVB', "Invalid tag at start of NavBlocker block ('NAVB' expected!)"))
    {
        SizeT num = reader->ReadUInt();
        IndexT i;
        for (i = 0; i < num; i++)
        {
            SizeT numPoints = reader->ReadUInt();
            IndexT j;
            for (j = 0; j < numPoints; j++)
            {
                reader->ReadFloat2();
            }
        }
        return true;        
    }
    return false;    
}

} // namespace Map
