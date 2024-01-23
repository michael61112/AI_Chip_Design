#include "systemc.h"
#include "../DT.h"
#include "../MON.h"
#include "../RAM.h"
#include "../ROM.h"
#include "cassert"
#include "cwr_dynamic_loader.h"
#include "cwr_sc_dynamic_stubs.h"
#include "cwr_sc_hierarch_module.h"
#include "cwr_sc_object_creator.h"
#include "scmlinc/scml_abstraction_level_switch.h"
#include "scmlinc/scml_property_registry.h"

using namespace conf;
using namespace std;


class DT0Creator : public ScObjectCreatorBase
{
public:
  static unsigned int creationVerboseMode() {
    const char * const env_var_val = ::getenv("SNPS_SLS_DYNAMIC_CREATION_VERBOSE");
    return env_var_val ? (::atoi(env_var_val)) : 3;
  }
  sc_object* create ( const string& name ) {
    string hierach_name = getHierarchicalName(name);
    if (scml_property_registry::inst().hasProperty(scml_property_registry::MODULE, scml_property_registry::BOOL, hierach_name, "runtime_disabled") && 
        scml_property_registry::inst().getBoolProperty(scml_property_registry::MODULE, hierach_name, "runtime_disabled")) {
      sc_module_name n(name.c_str());
      if (creationVerboseMode() >= 6) { std::cout << "DT/DT: STUB for " << hierach_name << " created." << std::endl; }
      conf::stub *result = new conf::stub(n);
       conf::stub_port_registrator<>::register_stub_port(&DT::clk, "clk" , string(static_cast<sc_object*>(result)->name()) + ".clk" );
       conf::stub_port_registrator<>::register_stub_port(&DT::reset, "reset" , string(static_cast<sc_object*>(result)->name()) + ".reset" );
       conf::stub_port_registrator<>::register_stub_port(&DT::sti_rd, "sti_rd" , string(static_cast<sc_object*>(result)->name()) + ".sti_rd" );
       conf::stub_port_registrator<>::register_stub_port(&DT::sti_addr, "sti_addr" , string(static_cast<sc_object*>(result)->name()) + ".sti_addr" );
       conf::stub_port_registrator<>::register_stub_port(&DT::sti_di, "sti_di" , string(static_cast<sc_object*>(result)->name()) + ".sti_di" );
       conf::stub_port_registrator<>::register_stub_port(&DT::res_wr, "res_wr" , string(static_cast<sc_object*>(result)->name()) + ".res_wr" );
       conf::stub_port_registrator<>::register_stub_port(&DT::res_rd, "res_rd" , string(static_cast<sc_object*>(result)->name()) + ".res_rd" );
       conf::stub_port_registrator<>::register_stub_port(&DT::res_addr, "res_addr" , string(static_cast<sc_object*>(result)->name()) + ".res_addr" );
       conf::stub_port_registrator<>::register_stub_port(&DT::res_do, "res_do" , string(static_cast<sc_object*>(result)->name()) + ".res_do" );
       conf::stub_port_registrator<>::register_stub_port(&DT::res_di, "res_di" , string(static_cast<sc_object*>(result)->name()) + ".res_di" );
       conf::stub_port_registrator<>::register_stub_port(&DT::done, "done" , string(static_cast<sc_object*>(result)->name()) + ".done" );
       conf::stub_port_registrator<>::register_stub_port(&DT::fwpass_finish, "fwpass_finish" , string(static_cast<sc_object*>(result)->name()) + ".fwpass_finish" );
      return result;
    } else {
      if (creationVerboseMode() >= 6) { std::cout << "DT/DT: " << hierach_name << " created." << std::endl; }
      DT* result = new DT(name.c_str());
       cwr_sc_object_registry::inst().addPort(&result->clk, string(static_cast<sc_object*>(result)->name()) + ".clk" );
       cwr_sc_object_registry::inst().addPort(&result->reset, string(static_cast<sc_object*>(result)->name()) + ".reset" );
       cwr_sc_object_registry::inst().addPort(&result->sti_rd, string(static_cast<sc_object*>(result)->name()) + ".sti_rd" );
       cwr_sc_object_registry::inst().addPort(&result->sti_addr, string(static_cast<sc_object*>(result)->name()) + ".sti_addr" );
       cwr_sc_object_registry::inst().addPort(&result->sti_di, string(static_cast<sc_object*>(result)->name()) + ".sti_di" );
       cwr_sc_object_registry::inst().addPort(&result->res_wr, string(static_cast<sc_object*>(result)->name()) + ".res_wr" );
       cwr_sc_object_registry::inst().addPort(&result->res_rd, string(static_cast<sc_object*>(result)->name()) + ".res_rd" );
       cwr_sc_object_registry::inst().addPort(&result->res_addr, string(static_cast<sc_object*>(result)->name()) + ".res_addr" );
       cwr_sc_object_registry::inst().addPort(&result->res_do, string(static_cast<sc_object*>(result)->name()) + ".res_do" );
       cwr_sc_object_registry::inst().addPort(&result->res_di, string(static_cast<sc_object*>(result)->name()) + ".res_di" );
       cwr_sc_object_registry::inst().addPort(&result->done, string(static_cast<sc_object*>(result)->name()) + ".done" );
       cwr_sc_object_registry::inst().addPort(&result->fwpass_finish, string(static_cast<sc_object*>(result)->name()) + ".fwpass_finish" );
      return result;
    }
  }
};

template <class BC>
class BusCreator : public ScObjectCreatorBase
{
  sc_object* create (const string& name) {
    ScGeneratedBaseBlockCreatorHelper helper(getHierarchicalName(name));
    if (helper.is_runtime_disabled()) {
      return new conf::StubBusCreator(name.c_str(), helper);
    } else {
      return new BC(name.c_str(), helper);
    }
  }
};

template <class GBC>
class GeneratedBlockCreator : public ScObjectCreatorBase
{
  sc_object* create (const string& name) {
    ScGeneratedBaseBlockCreatorHelper helper(getHierarchicalName(name));
    if (helper.is_runtime_disabled()) {
      return new conf::StubBusCreator(name.c_str(), helper);
    } else {
      return new GBC(name.c_str(), helper);
    }
  }
};



class MON0Creator : public ScObjectCreatorBase
{
public:
  static unsigned int creationVerboseMode() {
    const char * const env_var_val = ::getenv("SNPS_SLS_DYNAMIC_CREATION_VERBOSE");
    return env_var_val ? (::atoi(env_var_val)) : 3;
  }
  sc_object* create ( const string& name ) {
    string hierach_name = getHierarchicalName(name);
    if (scml_property_registry::inst().hasProperty(scml_property_registry::MODULE, scml_property_registry::BOOL, hierach_name, "runtime_disabled") && 
        scml_property_registry::inst().getBoolProperty(scml_property_registry::MODULE, hierach_name, "runtime_disabled")) {
      sc_module_name n(name.c_str());
      if (creationVerboseMode() >= 6) { std::cout << "MON/MON: STUB for " << hierach_name << " created." << std::endl; }
      conf::stub *result = new conf::stub(n);
       conf::stub_port_registrator<>::register_stub_port(&MON::clock, "clock" , string(static_cast<sc_object*>(result)->name()) + ".clock" );
       conf::stub_port_registrator<>::register_stub_port(&MON::fwpass_finish, "fwpass_finish" , string(static_cast<sc_object*>(result)->name()) + ".fwpass_finish" );
       conf::stub_port_registrator<>::register_stub_port(&MON::done, "done" , string(static_cast<sc_object*>(result)->name()) + ".done" );
       conf::stub_port_registrator<>::register_stub_port(&MON::cycle, "cycle" , string(static_cast<sc_object*>(result)->name()) + ".cycle" );
      return result;
    } else {
      if (creationVerboseMode() >= 6) { std::cout << "MON/MON: " << hierach_name << " created." << std::endl; }
      MON* result = new MON(name.c_str());
       cwr_sc_object_registry::inst().addPort(&result->clock, string(static_cast<sc_object*>(result)->name()) + ".clock" );
       cwr_sc_object_registry::inst().addPort(&result->fwpass_finish, string(static_cast<sc_object*>(result)->name()) + ".fwpass_finish" );
       cwr_sc_object_registry::inst().addPort(&result->done, string(static_cast<sc_object*>(result)->name()) + ".done" );
       cwr_sc_object_registry::inst().addPort(&result->cycle, string(static_cast<sc_object*>(result)->name()) + ".cycle" );
      return result;
    }
  }
};


class RAM0Creator : public ScObjectCreatorBase
{
public:
  static unsigned int creationVerboseMode() {
    const char * const env_var_val = ::getenv("SNPS_SLS_DYNAMIC_CREATION_VERBOSE");
    return env_var_val ? (::atoi(env_var_val)) : 3;
  }
  sc_object* create ( const string& name ) {
    string hierach_name = getHierarchicalName(name);
    if (scml_property_registry::inst().hasProperty(scml_property_registry::MODULE, scml_property_registry::BOOL, hierach_name, "runtime_disabled") && 
        scml_property_registry::inst().getBoolProperty(scml_property_registry::MODULE, hierach_name, "runtime_disabled")) {
      sc_module_name n(name.c_str());
      if (creationVerboseMode() >= 6) { std::cout << "RAM/RAM: STUB for " << hierach_name << " created." << std::endl; }
      conf::stub *result = new conf::stub(n);
       conf::stub_port_registrator<>::register_stub_port(&RAM::clock, "clock" , string(static_cast<sc_object*>(result)->name()) + ".clock" );
       conf::stub_port_registrator<>::register_stub_port(&RAM::res_rd, "res_rd" , string(static_cast<sc_object*>(result)->name()) + ".res_rd" );
       conf::stub_port_registrator<>::register_stub_port(&RAM::res_wr, "res_wr" , string(static_cast<sc_object*>(result)->name()) + ".res_wr" );
       conf::stub_port_registrator<>::register_stub_port(&RAM::res_addr, "res_addr" , string(static_cast<sc_object*>(result)->name()) + ".res_addr" );
       conf::stub_port_registrator<>::register_stub_port(&RAM::res_dataout, "res_dataout" , string(static_cast<sc_object*>(result)->name()) + ".res_dataout" );
       conf::stub_port_registrator<>::register_stub_port(&RAM::res_datain, "res_datain" , string(static_cast<sc_object*>(result)->name()) + ".res_datain" );
      return result;
    } else {
      if (creationVerboseMode() >= 6) { std::cout << "RAM/RAM: " << hierach_name << " created." << std::endl; }
      RAM* result = new RAM(name.c_str());
       cwr_sc_object_registry::inst().addPort(&result->clock, string(static_cast<sc_object*>(result)->name()) + ".clock" );
       cwr_sc_object_registry::inst().addPort(&result->res_rd, string(static_cast<sc_object*>(result)->name()) + ".res_rd" );
       cwr_sc_object_registry::inst().addPort(&result->res_wr, string(static_cast<sc_object*>(result)->name()) + ".res_wr" );
       cwr_sc_object_registry::inst().addPort(&result->res_addr, string(static_cast<sc_object*>(result)->name()) + ".res_addr" );
       cwr_sc_object_registry::inst().addPort(&result->res_dataout, string(static_cast<sc_object*>(result)->name()) + ".res_dataout" );
       cwr_sc_object_registry::inst().addPort(&result->res_datain, string(static_cast<sc_object*>(result)->name()) + ".res_datain" );
      return result;
    }
  }
};


class sti_ROM0Creator : public ScObjectCreatorBase
{
public:
  static unsigned int creationVerboseMode() {
    const char * const env_var_val = ::getenv("SNPS_SLS_DYNAMIC_CREATION_VERBOSE");
    return env_var_val ? (::atoi(env_var_val)) : 3;
  }
  sc_object* create ( const string& name ) {
    string hierach_name = getHierarchicalName(name);
    if (scml_property_registry::inst().hasProperty(scml_property_registry::MODULE, scml_property_registry::BOOL, hierach_name, "runtime_disabled") && 
        scml_property_registry::inst().getBoolProperty(scml_property_registry::MODULE, hierach_name, "runtime_disabled")) {
      sc_module_name n(name.c_str());
      if (creationVerboseMode() >= 6) { std::cout << "sti_ROM/sti_ROM: STUB for " << hierach_name << " created." << std::endl; }
      conf::stub *result = new conf::stub(n);
       conf::stub_port_registrator<>::register_stub_port(&sti_ROM::clock, "clock" , string(static_cast<sc_object*>(result)->name()) + ".clock" );
       conf::stub_port_registrator<>::register_stub_port(&sti_ROM::sti_rd, "sti_rd" , string(static_cast<sc_object*>(result)->name()) + ".sti_rd" );
       conf::stub_port_registrator<>::register_stub_port(&sti_ROM::sti_addr, "sti_addr" , string(static_cast<sc_object*>(result)->name()) + ".sti_addr" );
       conf::stub_port_registrator<>::register_stub_port(&sti_ROM::sti_data, "sti_data" , string(static_cast<sc_object*>(result)->name()) + ".sti_data" );
      return result;
    } else {
      if (creationVerboseMode() >= 6) { std::cout << "sti_ROM/sti_ROM: " << hierach_name << " created." << std::endl; }
      sti_ROM* result = new sti_ROM(name.c_str());
       cwr_sc_object_registry::inst().addPort(&result->clock, string(static_cast<sc_object*>(result)->name()) + ".clock" );
       cwr_sc_object_registry::inst().addPort(&result->sti_rd, string(static_cast<sc_object*>(result)->name()) + ".sti_rd" );
       cwr_sc_object_registry::inst().addPort(&result->sti_addr, string(static_cast<sc_object*>(result)->name()) + ".sti_addr" );
       cwr_sc_object_registry::inst().addPort(&result->sti_data, string(static_cast<sc_object*>(result)->name()) + ".sti_data" );
      return result;
    }
  }
};



extern void scv_tr_cwrdb_init();

int sc_main(int argc, char* argv[])
{
  scv_tr_cwrdb_init();
  ScObjectFactory::inst().addCreator ("DT0", new DT0Creator());
  ScObjectFactory::inst().addCreator ("MON0", new MON0Creator());
  ScObjectFactory::inst().addCreator ("RAM0", new RAM0Creator());
  ScObjectFactory::inst().addCreator ("sc_signal<bool>", new ScPrimChannelCreator<sc_signal<bool> >());
  ScObjectFactory::inst().addCreator ("sc_signal<int>", new ScPrimChannelCreator<sc_signal<int> >());
  ScObjectFactory::inst().addCreator ("sc_signal<sc_uint<10> >", new ScPrimChannelCreator<sc_signal<sc_uint<10> > >());
  ScObjectFactory::inst().addCreator ("sc_signal<sc_uint<14> >", new ScPrimChannelCreator<sc_signal<sc_uint<14> > >());
  ScObjectFactory::inst().addCreator ("sc_signal<sc_uint<16> >", new ScPrimChannelCreator<sc_signal<sc_uint<16> > >());
  ScObjectFactory::inst().addCreator ("sc_signal<sc_uint<8> >", new ScPrimChannelCreator<sc_signal<sc_uint<8> > >());
  ScObjectFactory::inst().addCreator ("sti_ROM0", new sti_ROM0Creator());
  ScPortFactory::inst().addCreator ("sc_in<bool>", new ScPortCreator<sc_in<bool> >());
  ScPortFactory::inst().addCreator ("sc_in<int>", new ScPortCreator<sc_in<int> >());
  ScPortFactory::inst().addCreator ("sc_in<sc_uint<10> >", new ScPortCreator<sc_in<sc_uint<10> > >());
  ScPortFactory::inst().addCreator ("sc_in<sc_uint<14> >", new ScPortCreator<sc_in<sc_uint<14> > >());
  ScPortFactory::inst().addCreator ("sc_in<sc_uint<16> >", new ScPortCreator<sc_in<sc_uint<16> > >());
  ScPortFactory::inst().addCreator ("sc_in<sc_uint<8> >", new ScPortCreator<sc_in<sc_uint<8> > >());
  ScPortFactory::inst().addCreator ("sc_inout<bool>", new ScPortCreator<sc_inout<bool> >());
  ScPortFactory::inst().addCreator ("sc_inout<int>", new ScPortCreator<sc_inout<int> >());
  ScPortFactory::inst().addCreator ("sc_inout<sc_uint<10> >", new ScPortCreator<sc_inout<sc_uint<10> > >());
  ScPortFactory::inst().addCreator ("sc_inout<sc_uint<14> >", new ScPortCreator<sc_inout<sc_uint<14> > >());
  ScPortFactory::inst().addCreator ("sc_inout<sc_uint<16> >", new ScPortCreator<sc_inout<sc_uint<16> > >());
  ScPortFactory::inst().addCreator ("sc_inout<sc_uint<8> >", new ScPortCreator<sc_inout<sc_uint<8> > >());
  ScPortFactory::inst().addCreator ("sc_out<bool>", new ScPortCreator<sc_out<bool> >());
  ScPortFactory::inst().addCreator ("sc_out<int>", new ScPortCreator<sc_out<int> >());
  ScPortFactory::inst().addCreator ("sc_out<sc_uint<10> >", new ScPortCreator<sc_out<sc_uint<10> > >());
  ScPortFactory::inst().addCreator ("sc_out<sc_uint<14> >", new ScPortCreator<sc_out<sc_uint<14> > >());
  ScPortFactory::inst().addCreator ("sc_out<sc_uint<16> >", new ScPortCreator<sc_out<sc_uint<16> > >());
  ScPortFactory::inst().addCreator ("sc_out<sc_uint<8> >", new ScPortCreator<sc_out<sc_uint<8> > >());
  if (::getenv("SNPS_SLS_DYNAMIC_LOADER_VERBOSE")) { std::cout << "DT/DT loaded." << std::endl; }
  if (::getenv("SNPS_SLS_DYNAMIC_LOADER_VERBOSE")) { std::cout << "MON/MON loaded." << std::endl; }
  if (::getenv("SNPS_SLS_DYNAMIC_LOADER_VERBOSE")) { std::cout << "RAM/RAM loaded." << std::endl; }
  if (::getenv("SNPS_SLS_DYNAMIC_LOADER_VERBOSE")) { std::cout << "sti_ROM/sti_ROM loaded." << std::endl; }

  scml_property_registry::inst().setXMLFile("Properties.xml");

  nSnps::nDynamicLoading::DynamicLoader dynamicLoader
    (scml_property_registry::inst().getStringProperty(scml_property_registry::GLOBAL, "", "/pct/runtime/dynamic_load_info"));

  string xml_file_path = scml_property_registry::inst().getPropertyXmlFilePath();
  string hierarchy_xml_file = xml_file_path + "/" + 
    scml_property_registry::inst().getStringProperty(
      scml_property_registry::GLOBAL, "", "system_hierarchy_xml_file");

  cwr_sc_hierarch_module* HARDWARE = new cwr_sc_hierarch_module("HARDWARE", hierarchy_xml_file);

  // Enable/disable backdoor accesses
  scml_abstraction_level_switch::instance().set_simulation_mode(scml_property_registry::inst().getStringProperty(scml_property_registry::GLOBAL, "", "/pct/runtime/simulation_mode")=="MODE_FULL_SIMULATION" ? scml_abstraction_level_switch::MODE_FULL_SIMULATION : scml_abstraction_level_switch::MODE_SPEED_OPTIMIZED);

  sc_start();

  if (sc_is_running()) {
    sc_stop();
  }
  delete HARDWARE;

  return 0;
}
