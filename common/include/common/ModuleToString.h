#pragma once

namespace spvgentwo
{
	// forward decl
	class Module;
	class Grammar;
	class String;
	class IAllocator;

	class IModulePrinter
	{
	public:
		// expects ANSI color codes, nllptr does nothing
		virtual void append(const char* _pText, const char* _pushColor = nullptr, const char* _popColor = nullptr) = 0;
		virtual void append(unsigned int _literal, const char* _pushColor = nullptr, const char* _popColor = nullptr) = 0;

		IModulePrinter& operator<<(const char* _pStr) {	append(_pStr); return *this;}
		IModulePrinter& operator<<(unsigned int _literal) { append(_literal); return *this; }
		//template <unsigned int Size>
		//IModulePrinter& operator<<(const char(&_arr)[Size]) { append(_arr); return *this; }
	};

	class ModuleStringPrinter : public IModulePrinter
	{
	public:
		ModuleStringPrinter(String& _outBuffer) : m_buffer(_outBuffer) {}

		void append(const char* _pStr, const char* _pushColor = nullptr, const char* _popColor = nullptr) final;
		void append(unsigned int _literal, const char* _pushColor = nullptr, const char* _popColor = nullptr) final;

	private:
		String& m_buffer;
	};

	bool moduleToString(Module& _module, const Grammar& _grammar, IAllocator* _pAlloc, IModulePrinter* _pOutput);
}