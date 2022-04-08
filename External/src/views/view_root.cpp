#include "view.hpp"

namespace ext
{
	void view::root()
	{
		view::heading();
		view::navigation();
		view::players();
		view::active_view();
	}
}
