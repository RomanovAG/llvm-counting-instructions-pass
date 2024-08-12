#include "llvm/IR/InstIterator.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

void
VisitFunction(const Function &Func)
{
	DenseMap<unsigned, unsigned> inst_count;
	
	for (inst_iterator I = inst_begin(Func), E = inst_end(Func); I != E; I++)
	{
		inst_count[I->getOpcode()]++;
	}

	outs() << "Function " << Func.getName() << ":\n";
	for (const auto &inst : inst_count)
	{
		outs() << "\t" << Instruction::getOpcodeName(inst.first) << ":\t" << inst.second << "\n";
	}
}

struct CountingInstructionsPass : PassInfoMixin<CountingInstructionsPass> 
{
	PreservedAnalyses 
	run(Function &Function, FunctionAnalysisManager &AnalysisManager) 
	{
		VisitFunction(Function);
		return (PreservedAnalyses::all());
	}

	static bool 
	isRequired(void) 
	{ 
		return (true); 
	}
};
} /* namespace */

void
CallBackForPassBuilder(PassBuilder &PB)
{
	PB.registerPipelineParsingCallback(
		(	[](
			StringRef Name,
			FunctionPassManager &FPM,
			ArrayRef<PassBuilder::PipelineElement>
			) -> bool
			{
				if (Name == "counting-instructions-pass")
				{
					FPM.addPass(CountingInstructionsPass());
					return (true);
				}
				else
				{
					return (false);
				}
			}
		)
	);
} /* CallBackForPassBuider */

PassPluginLibraryInfo 
getCountingInstructionsPassPluginInfo(void)
{
	uint32_t     APIversion =  LLVM_PLUGIN_API_VERSION;
	const char * PluginName =  "counting-instructions-pass";
	const char * PluginVersion =  LLVM_VERSION_STRING;
    
	PassPluginLibraryInfo info = 
	{ 
		APIversion, 
		PluginName, 
		PluginVersion, 
		CallBackForPassBuilder
	};

	return (info);
} /* getCountingInstructionsPassPluginInfo */

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() 
{
	return (getCountingInstructionsPassPluginInfo());
} /* llvmGetPassPluginInfo */
