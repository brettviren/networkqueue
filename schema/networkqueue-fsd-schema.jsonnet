// The schema used by classes in the appfwk code tests.
//
// It is an example of the lowest layer schema below that of the "cmd"
// and "app" and which defines the final command object structure as
// consumed by instances of specific DAQModule implementations (ie,
// the test/Fake* modules).

local moo = import "moo.jsonnet";

// A schema builder in the given path (namespace)
local ns = "dunedaq.networkqueue.fsd";
local s = moo.oschema.schema(ns);

// Object structure used by the test/fake producer module
local fsd = {
  count : s.number("Count", "i4",
    doc="A count of not too many things"),

  timestamp : s.number("Timestamp", "i8",
    doc="A fake timestamp"),
  
  fakedata: s.record("FakeData", [
    s.field("fake_count", self.count, -4,
      doc="A fake count of something"),
  ], doc="Fake Serializable data"),
  
  fakedata2: s.record("AnotherFakeData", [
    s.field("fake_count", self.count, -4,
      doc="A fake count of something"),
    s.field("fake_timestamp", self.timestamp, 0,
      doc="A fake timestamp for the data"),
  ], doc="Another fake Serializable data"),

};

moo.oschema.sort_select(fsd, ns)

