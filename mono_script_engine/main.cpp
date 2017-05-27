#include <iostream>
#include <string>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

void DoSomething()
{
	std::cout << "DoSomething" << std::endl;
}

void TestSimple(MonoDomain* domain, MonoAssembly* assembly, MonoImage* image)
{

	MonoClass* testclass = mono_class_from_name(image, "TestNamespace", "TestClass");
	MonoMethodDesc* methoddesc = mono_method_desc_new("TestClass::Print", false);
	MonoMethod* method = mono_method_desc_search_in_class(methoddesc, testclass);
	mono_runtime_invoke(method, NULL, NULL, NULL);
}


void TestDog(MonoDomain* domain, MonoAssembly* assembly, MonoImage* image)
{
	MonoClass* dogclass = mono_class_from_name(image, "cs_dll", "Dog");
	MonoObject* dogA = mono_object_new(domain, dogclass);
	//Call its default constructor
	mono_runtime_object_init(dogA);
	//mono_gchandle_new() to add gc reference,to avoid deconstruct
	//mono_gchandle_free() to decrease gc reference


	MonoMethodDesc* BarkMethodDesc = mono_method_desc_new("Dog:Bark(int)", false); ;
	MonoMethod* bark_method = mono_method_desc_search_in_class(BarkMethodDesc, dogclass);
	//Set the arguments for the method
	void* args[1];
	int barkTimes = 3;
	args[0] = &barkTimes;
	mono_runtime_invoke(bark_method, dogA, args, NULL);
}

int main()
{
	std::string input_value;
	MonoDomain* domain = mono_jit_init_version("ScriptDomain", "v4.0.30319");
	MonoAssembly* assembly = mono_domain_assembly_open(domain, "../cs_script_dll/cs_dll.dll");
	mono_add_internal_call("ScriptAPI::DoSomething", &DoSomething);
	MonoImage* image = mono_assembly_get_image(assembly);
	if (!assembly)
	{
		std::cout << "err" << std::endl;
		abort();
	}
	
	TestDog(domain, assembly, image);
	TestSimple(domain, assembly, image);
	mono_jit_cleanup(domain);
	return 0;
}